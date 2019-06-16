//! This file contains signal interface

use super::process::CpuState;
use super::SysResult;

use alloc::collections::vec_deque::VecDeque;
use bit_field::BitField;
use bitflags::bitflags;
use core::convert::TryFrom;
use core::mem;
use core::mem::transmute;
use core::ops::{BitOr, BitOrAssign, Index, IndexMut};
use errno::Errno;

#[derive(Debug, Copy, Clone, PartialEq)]
#[allow(dead_code)]
#[repr(u32)]
pub enum Signum {
    SigNull = 0,
    /// Hangup (POSIX).
    Sighup = 1,
    /// Interrupt (ANSI).
    Sigint = 2,
    /// Quit (POSIX).
    Sigquit = 3,
    /// Illegal instruction (ANSI).
    Sigill = 4,
    /// Trace trap (POSIX).
    Sigtrap = 5,
    /// Abort (ANSI).
    Sigabrt = 6,
    /// BUS error (4.2 BSD).
    Sigbus = 7,
    /// Floating-point exception (ANSI).
    Sigfpe = 8,
    /// Kill, unblockable (POSIX).
    Sigkill = 9,
    /// User-defined signal 1 (POSIX).
    Sigusr1 = 10,
    /// Segmentation violation (ANSI).
    Sigsegv = 11,
    /// User-defined signal 2 (POSIX).
    Sigusr2 = 12,
    /// Broken pipe (POSIX).
    Sigpipe = 13,
    /// Alarm clock (POSIX).
    Sigalrm = 14,
    /// Termination (ANSI).
    Sigterm = 15,
    /// Stack fault.
    Sigstkflt = 16,
    /// Child status has changed (POSIX).
    Sigchld = 17,
    /// Continue (POSIX).
    Sigcont = 18,
    /// Stop, unblockable (POSIX).
    Sigstop = 19,
    /// Keyboard stop (POSIX).
    Sigtstp = 20,
    /// Background read from tty (POSIX).
    Sigttin = 21,
    /// Background write to tty (POSIX).
    Sigttou = 22,
    /// Urgent condition on socket (4.2 BSD).
    Sigurg = 23,
    /// CPU limit exceeded (4.2 BSD).
    Sigxcpu = 24,
    /// File size limit exceeded (4.2 BSD).
    Sigxfsz = 25,
    /// Virtual alarm clock (4.2 BSD).
    Sigvtalrm = 26,
    /// Profiling alarm clock (4.2 BSD).
    Sigprof = 27,
    /// Window size change (4.3 BSD, Sun).
    Sigwinch = 28,
    /// I/O now possible (4.2 BSD).
    Sigio = 29,
    /// Power failure restart (System V).
    Sigpwr = 30,
    /// Bad system call.
    Sigsys = 31,
}

#[derive(Copy, Clone, PartialEq, Eq)]
pub enum DefaultAction {
    Abort,
    Terminate,
    Ignore,
    Stop,
    Continue,
}

pub fn signal_default_action(signum: Signum) -> DefaultAction {
    use Signum::*;
    match signum {
        Sigstkflt | Sigabrt | Sigbus | Sigfpe | Sigill | Sigquit | Sigsegv | Sigsys | Sigtrap | Sigxcpu | Sigxfsz => {
            DefaultAction::Abort
        }

        Sigalrm | Sighup | Sigint | Sigkill | Sigpipe | Sigusr1 | Sigusr2 | Sigprof | Sigvtalrm | Sigterm => {
            DefaultAction::Terminate
        }

        // Special, may have 2 behavior: Continuing exection AND/OR execute handler. Cannot be ignored
        Sigcont => DefaultAction::Continue,

        Sigio | Sigpwr | Sigwinch | SigNull | Sigchld | Sigurg => DefaultAction::Ignore,

        // SIGSTOP cannot be handled. For others, we can STOP process execution OR EXECUTE handler
        Sigstop | Sigtstp | Sigttin | Sigttou => DefaultAction::Stop,
    }
}

#[derive(Debug)]
pub struct InvalidSignum;

impl TryFrom<u32> for Signum {
    type Error = InvalidSignum;
    fn try_from(n: u32) -> Result<Self, Self::Error> {
        if n >= 32 {
            return Err(InvalidSignum);
        } else {
            Ok(unsafe { transmute(n) })
        }
    }
}

#[derive(Copy, Clone, Debug, Default)]
#[repr(transparent)]
pub struct SaMask(u32);

impl SaMask {
    fn contains(&self, signum: Signum) -> bool {
        self.0.get_bit(signum as u32 as usize)
    }
}

impl From<Signum> for SaMask {
    fn from(s: Signum) -> Self {
        Self(1 << s as u32)
    }
}

impl BitOr for SaMask {
    type Output = Self;

    fn bitor(self, rhs: Self) -> Self::Output {
        Self(self.0 | rhs.0)
    }
}

impl BitOrAssign for SaMask {
    fn bitor_assign(&mut self, rhs: Self) {
        *self = *self | rhs;
    }
}

const SIG_DFL: usize = 0;
const SIG_IGN: usize = 1;

#[derive(Copy, Clone, Debug)]
#[repr(C)]
pub struct StructSigaction {
    pub sa_handler: usize,
    // TODO: Must be an union with sa_handler
    // sa_sigaction: extern "C" fn(int, siginfo_t *, void *),
    pub sa_mask: SaMask,
    pub sa_flags: SaFlags,
    pub sa_restorer: usize,
}

bitflags! {
    pub struct SaFlags: u32 {
        const SA_NOCLDSTOP = 0x00000001;
        const SA_NOCLDWAIT = 0x00000002;
        const SA_SIGINFO   = 0x00000004;
        const SA_ONSTACK   = 0x08000000;
        const SA_RESTART   = 0x10000000;
        const SA_NODEFER   = 0x40000000;
        const SA_RESETHAND = 0x80000000;
        const SA_RESTORER  = 0x04000000;
    }
}

impl Default for StructSigaction {
    fn default() -> Self {
        Self { sa_handler: SIG_DFL, sa_mask: Default::default(), sa_flags: SaFlags::SA_RESTART, sa_restorer: 0 }
    }
}

#[derive(Copy, Clone, Debug)]
pub struct SignalActions(pub [StructSigaction; 32]);

impl IndexMut<Signum> for SignalActions {
    fn index_mut(&mut self, index: Signum) -> &mut StructSigaction {
        &mut self.0[index as usize]
    }
}

impl Index<Signum> for SignalActions {
    type Output = StructSigaction;
    fn index(&self, index: Signum) -> &Self::Output {
        &self.0[index as usize]
    }
}

#[derive(Debug)]
pub struct SignalInterface {
    pub signal_actions: SignalActions,
    pub signal_queue: VecDeque<Signum>,
    pub current_sa_mask: SaMask,
    pub depth: u32,
}

bitflags! {
    /// The check_pending_signals function returns what to do
    #[derive(Default)]
    pub struct JobAction: u32 {
        /// A signal must be handled
        const INTERRUPT = 1 << 0;
        /// A deadly signal was throw
        const TERMINATE = 1 << 1;
        /// The job must stop
        const STOP = 1 << 2;
        /// The job must continue
        const CONTINUE = 1 << 3;
    }
}

impl SignalInterface {
    /// Create a new signal Inteface
    pub fn new() -> Self {
        Self {
            signal_actions: SignalActions([Default::default(); 32]),
            signal_queue: VecDeque::new(),
            current_sa_mask: Default::default(),
            depth: 0,
        }
    }

    /// Fork the signal interface
    pub fn fork(&self) -> Self {
        Self {
            signal_actions: SignalActions([Default::default(); 32]),
            //The set of signals pending for the child process shall be
            //initialized to the empty set.
            signal_queue: VecDeque::new(),
            //The signal mask for a thread shall be initialized from that of its
            //parent or creating thread, or from the corresponding thread in the
            //parent process if the thread was created as the result of a call to
            //fork()
            current_sa_mask: self.current_sa_mask,
            depth: 0,
        }
    }

    /// Get a Job action(s) relative to signal_queue content
    /// This function is non-mutable
    pub fn get_job_action(&self) -> JobAction {
        let mut action: JobAction = JobAction::default();

        for &signum in self.signal_queue.iter() {
            if self.current_sa_mask.contains(signum) {
                continue;
            }
            let sigaction = self.signal_actions[signum];
            action |= match sigaction.sa_handler {
                SIG_IGN => JobAction::default(),
                SIG_DFL => {
                    use DefaultAction::*;
                    match signal_default_action(signum) {
                        Abort | Terminate => JobAction::TERMINATE,
                        Continue => JobAction::CONTINUE,
                        Stop => JobAction::STOP,
                        Ignore => JobAction::default(),
                    }
                }
                _ => {
                    if signum == Signum::Sigcont {
                        JobAction::INTERRUPT | JobAction::CONTINUE
                    } else {
                        JobAction::INTERRUPT
                    }
                }
            };
        }
        action
    }

    /// Create handler contexts and pop all the signal queue. Return Some(signum) in case of Deadly signal
    pub fn exec_signal_handler(&mut self, cpu_state: *mut CpuState, in_blocked_syscall: bool) -> Option<Signum> {
        for &signum in self.signal_queue.iter() {
            if self.current_sa_mask.contains(signum) {
                continue;
            }
            let sigaction = self.signal_actions[signum];
            match sigaction.sa_handler {
                SIG_IGN => {}
                SIG_DFL => {
                    use DefaultAction::*;
                    match signal_default_action(signum) {
                        Abort | Terminate => return Some(signum),
                        _ => {}
                    }
                }
                _ => {
                    // Only the RESTART of the first signal is considered
                    if self.depth == 0 && in_blocked_syscall {
                        if sigaction.sa_flags.contains(SaFlags::SA_RESTART) {
                            // back 2 instruction to reput eip on `int 80h` and restart the syscall
                            unsafe { (*cpu_state).eip -= 2 };
                        } else {
                            // else the syscall must return Eintr
                            unsafe { (*cpu_state).registers.eax = (-(Errno::Eintr as i32)) as u32 };
                        }
                    }
                    self.depth += 1;
                    unsafe {
                        context_builder::push(cpu_state, self.current_sa_mask, signum, sigaction.sa_handler as u32);
                    }
                    self.current_sa_mask = self.current_sa_mask | sigaction.sa_mask;
                    if !sigaction.sa_flags.contains(SaFlags::SA_NODEFER) {
                        self.current_sa_mask |= SaMask::from(signum);
                    }
                }
            };
        }
        self.signal_queue.truncate(0);
        None
    }

    /// Acknowledge end of signal execution, pop the first internal signal and a restore context form the signal frame.
    pub fn terminate_pending_signal(&mut self, process_context_ptr: u32) {
        self.depth -= 1;
        unsafe {
            self.current_sa_mask = context_builder::pop(process_context_ptr as *mut CpuState);
        }
    }

    /// Register a new handler for a specified Signum
    pub fn new_handler(&mut self, signum: Signum, sigaction: &StructSigaction) -> SysResult<u32> {
        //The system shall not allow the action for the signals
        // SIGKILL or SIGSTOP to be set to SIG_IGN.
        if (signum == Signum::Sigkill || signum == Signum::Sigstop) && sigaction.sa_handler != SIG_DFL {
            return Err(Errno::Einval);
        }
        // SIGCONT cannot be ignored (job control mandatory)
        if signum == Signum::Sigcont && sigaction.sa_handler == SIG_IGN {
            return Err(Errno::Einval);
        }

        // Associate a new action for a specified Signum
        let former = mem::replace(&mut self.signal_actions[signum], *sigaction);
        Ok(former.sa_handler as u32)
    }

    /// Register a new signal
    pub fn generate_signal(&mut self, signum: Signum) -> SysResult<u32> {
        // If the same signal already exists in signal queue, ignore it
        if self.signal_queue.iter().any(|&s| s == signum) {
            return Ok(0);
        }

        // When any stop signal (SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU) is generated
        // for a process or thread, all pending SIGCONT signals for that process
        // or any of the threads within that process shall be
        // discarded.
        if signal_default_action(signum) == DefaultAction::Stop {
            self.signal_queue.retain(|&s| s != Signum::Sigcont);
        }

        // Conversely, when SIGCONT is generated for a process or
        // thread, all pending stop signals for that process or any of the
        // threads within that process shall be discarded
        if signal_default_action(signum) == DefaultAction::Continue {
            self.signal_queue.retain(|&s| signal_default_action(s) != DefaultAction::Stop);
        }

        self.signal_queue.try_reserve(1)?;
        self.signal_queue.push_back(signum);
        Ok(0)
    }
}

/// This module allow to create contexts for handlers and to get back from them
mod context_builder {
    use super::{CpuState, SaMask, Signum};

    use core::mem::size_of;

    /// Create a new context witch will execute a signal handler
    pub unsafe fn push(cpu_state: *mut CpuState, sa_mask: SaMask, signum: Signum, handler_address: u32) {
        let mut user_esp = (*cpu_state).esp;

        /* PUSH DATA SECTION */

        // push the current cpu_state on the user stack
        push_esp(&mut user_esp, *cpu_state);

        // push the sa_mask
        push_esp(&mut user_esp, sa_mask);

        // push the trampoline code on the user stack
        push_buff_esp(&mut user_esp, symbol_addr!(_trampoline) as *mut u8, _trampoline_len as usize);

        // push the address of start of trampoline code stack on the user stack
        let eip_trampoline = user_esp;
        push_esp(&mut user_esp, signum as u32);
        push_esp(&mut user_esp, eip_trampoline);

        (*cpu_state).eip = handler_address;
        (*cpu_state).esp = user_esp;
    }

    /// Destroy a context and set execution pointer on the previous context. Return the stored SA_MASK
    pub unsafe fn pop(cpu_state: *mut CpuState) -> SaMask {
        // skip the trampoline code
        (*cpu_state).esp += align_on(_trampoline_len as usize, 4) as u32;

        // skip Signum: eq to `add esp, 4`
        (*cpu_state).esp += 4;

        /* POP DATA SECTION */

        let sa_mask = pop_esp(&mut (*cpu_state).esp);

        // secure restore stored registers (GDT selectors are exclude)
        let old_cpu_state: CpuState = pop_esp(&mut (*cpu_state).esp);
        (*cpu_state).registers = old_cpu_state.registers;
        (*cpu_state).eip = old_cpu_state.eip;
        (*cpu_state).esp = old_cpu_state.esp;
        (*cpu_state).eflags = old_cpu_state.eflags;

        // return stored sa_mask
        sa_mask
    }

    /// helper to push on the stack
    /// imitate push instruction by incrementing esp before push t
    fn push_esp<T: Copy>(esp: &mut u32, t: T) {
        if size_of::<T>() % 4 != 0 {
            panic!("size not multiple of 4");
        }
        *esp -= size_of::<T>() as u32;
        unsafe {
            (*esp as *mut T).write(t);
        }
    }

    /// helper to push on the stack
    /// same as push_esp but buf a `buf` of size `size`
    fn push_buff_esp(esp: &mut u32, buf: *mut u8, size: usize) {
        // align size
        let size = align_on(size, 4);
        *esp -= size as u32;
        unsafe {
            (*esp as *mut u8).copy_from(buf, size);
        }
    }

    /// helper to pop on the stack
    /// imitate pop instruction return the T present at esp
    fn pop_esp<T: Copy>(esp: &mut u32) -> T {
        if size_of::<T>() % 4 != 0 {
            panic!("size not multiple of 4");
        }
        unsafe {
            let t = *(*esp as *mut T);
            *esp += size_of::<T>() as u32;
            t
        }
    }

    /// align on
    #[inline(always)]
    fn align_on(t: usize, on: usize) -> usize {
        debug_assert!(on.is_power_of_two());
        if t & (on - 1) == 0 {
            t
        } else {
            t + (on - (t & (on - 1)))
        }
    }

    /// Extern ASM trampoline function for stack smaching
    extern "C" {
        static _trampoline: u8;
        static _trampoline_len: u32;
    }
}
