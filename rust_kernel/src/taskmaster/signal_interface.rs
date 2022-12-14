//! This file contains signal interface

use super::process::CpuState;
use super::SysResult;

use alloc::collections::vec_deque::VecDeque;
use bit_field::BitField;
use bitflags::bitflags;
use core::mem;
use core::ops::{BitAnd, BitOr, BitOrAssign, Index, IndexMut, Not};
use libc_binding::Errno;
use libc_binding::Signum;
use libc_binding::{
    SA_NOCLDSTOP, SA_NOCLDWAIT, SA_NODEFER, SA_ONSTACK, SA_RESETHAND, SA_RESTART, SA_RESTORER,
    SA_SIGINFO,
};
use libc_binding::{SIG_BLOCK, SIG_SETMASK, SIG_UNBLOCK};

use crate::memory::tools::PAGE_SIZE;

#[allow(non_camel_case_types)]
pub type sigset_t = u32;

#[repr(u32)]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum DefaultAction {
    Abort,
    Terminate,
    Ignore,
    Stop,
    Continue,
}

/// From boilerplate to get default action of a specified signum
impl core::convert::From<Signum> for DefaultAction {
    fn from(signum: Signum) -> Self {
        use Signum::*;
        match signum {
            // These signals are deadly by default. The behavior of a process is undefined after it ignores a
            // SIGFPE, SIGILL, SIGSEGV, or SIGBUS signal that was not generated by kill(), sigqueue(), or raise().
            SIGSTKFLT | SIGABRT | SIGBUS | SIGFPE | SIGILL | SIGQUIT | SIGSEGV | SIGSYS
            | SIGTRAP | SIGXCPU | SIGXFSZ => DefaultAction::Abort,

            // These signals are deadly by default. Only SIGKILL cannot be ignored or handled.
            SIGALRM | SIGHUP | SIGINT | SIGKILL | SIGPIPE | SIGUSR1 | SIGUSR2 | SIGPROF
            | SIGVTALRM | SIGTERM => DefaultAction::Terminate,

            // The default action for SIGCONT is to resume execution at the point where the process
            // was stopped, after first handling any pending unblocked signals.
            SIGCONT => DefaultAction::Continue,

            // These signals are ignored by default, they can be handled.
            SIGIO | SIGPWR | SIGWINCH | SIGNULL | SIGCHLD | SIGURG => DefaultAction::Ignore,

            // SIGSTOP cannot be handled or ignore. For others, they can STOP process execution, or execute handler ot be ignored.
            SIGSTOP | SIGTSTP | SIGTTIN | SIGTTOU => DefaultAction::Stop,
        }
    }
}

#[derive(Copy, Clone, Debug, Default)]
#[repr(transparent)]
pub struct SaMask(u32);

impl SaMask {
    /// Check is sa_mask contains a specified signum
    fn contains(&self, s: Signum) -> bool {
        self.0.get_bit(s as u32 as usize)
    }

    /// Check if the current Signum is marked as masked. Ignore it if SIGSTOP, SIGKILL and SIGCONT for job control
    #[inline(always)]
    fn is_masked(&self, s: Signum) -> bool {
        self.contains(s) && s != Signum::SIGSTOP && s != Signum::SIGKILL && s != Signum::SIGCONT
    }

    /// Udpdate sa_mask relative to a sigaction sa_mask and signum if no defer.
    fn update(&mut self, s: Signum, sigaction: &StructSigaction) {
        *self = *self | sigaction.sa_mask;
        if !sigaction.sa_flags.intersects(SaFlags::SA_NODEFER) {
            *self |= SaMask::from(s);
        }
    }
}

impl From<Signum> for SaMask {
    fn from(s: Signum) -> Self {
        Self(1 << s as u32)
    }
}

impl From<u32> for SaMask {
    fn from(n: u32) -> Self {
        // make sure we don't create a mask with syskill or Sigstop as
        // syskill and sigstop cannot be blocked
        SaMask(
            n & !(Signum::SIGKILL as u32) & !(Signum::SIGSTOP as u32) & !(Signum::SIGCONT as u32),
        )
    }
}

impl BitOr for SaMask {
    type Output = Self;

    fn bitor(self, rhs: Self) -> Self::Output {
        Self(self.0 | rhs.0)
    }
}

impl Not for SaMask {
    type Output = Self;

    fn not(self) -> Self::Output {
        Self(!self.0)
    }
}

impl BitAnd for SaMask {
    type Output = Self;

    fn bitand(self, rhs: Self) -> Self::Output {
        Self(self.0 & rhs.0)
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
    /// is an union with
    /// sa_sigaction: extern "C" fn(int, siginfo_t *, void *),
    pub sa_handler: usize,
    pub sa_mask: SaMask,
    pub sa_flags: SaFlags,
    pub sa_restorer: usize,
}

bitflags! {
    #[derive(Default)]
    pub struct SaFlags: u32 {
        const SA_NOCLDSTOP = SA_NOCLDSTOP;
        const SA_NOCLDWAIT = SA_NOCLDWAIT;
        const SA_SIGINFO   = SA_SIGINFO;
        const SA_RESTORER  = SA_RESTORER;
        const SA_ONSTACK   = SA_ONSTACK;
        const SA_RESTART   = SA_RESTART;
        const SA_NODEFER   = SA_NODEFER;
        const SA_RESETHAND = SA_RESETHAND;
    }
}

impl Default for StructSigaction {
    fn default() -> Self {
        Self {
            sa_handler: SIG_DFL,
            sa_mask: Default::default(),
            sa_flags: SaFlags::default(),
            sa_restorer: 0,
        }
    }
}

impl StructSigaction {
    fn is_handled(&self) -> bool {
        self.sa_handler != SIG_DFL && self.sa_handler != SIG_IGN
    }
}

#[derive(Copy, Clone, Debug)]
struct SignalActions(pub [StructSigaction; 32]);

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

/// Main Signal Inteface structure
#[derive(Debug)]
pub struct SignalInterface {
    /// This array contains all the actions which must be applied for each signums
    signal_actions: SignalActions,
    /// This is the queue of all pending signals
    signal_queue: VecDeque<Signum>,
    /// The SaMask specifies a mask of signals which should be blocked
    current_sa_mask: SaMask,
}

bitflags! {
    /// The check_pending_signals function returns what to do
    #[derive(Default)]
    pub struct JobAction: u32 {
        /// A signal must be handled
        const INTERRUPT = 1 << 0;
        /// A deadly signal was throw
        const TERMINATE = 1 << 1;
        /// The job must stop (STOP state may be removed by SIGCONT)
        const STOP = 1 << 2;
    }
}

impl SignalInterface {
    /// Create a new signal Inteface
    pub fn new() -> Self {
        Self {
            signal_actions: SignalActions([Default::default(); 32]),
            signal_queue: VecDeque::new(),
            current_sa_mask: Default::default(),
        }
    }

    /// Used to reset the signals after a call to execve
    pub fn reset_for_new_process_image(&mut self) {
        // Signals set to the default action (SIG_DFL) in the calling
        // process image shall be set to the default action in the new
        // process image. Except for SIGCHLD, signals set to be
        // ignored (SIG_IGN) by the calling process image shall be set
        // to be ignored by the new process image. Signals set to be
        // caught by the calling process image shall be set to the
        // default action in the new process image (see <signal.h>).

        // If the SIGCHLD signal is set to be ignored by the calling
        // process image, it is unspecified whether the SIGCHLD signal
        // is set to be ignored or to the default action in the new
        // process image.
        for sigaction in &mut self.signal_actions.0 {
            if sigaction.is_handled() {
                *sigaction = Default::default();
            }
            // After a successful call to any of the exec functions,
            // alternate signal stacks are not preserved and the
            // SA_ONSTACK flag shall be cleared for all signals.
            sigaction.sa_flags.remove(SaFlags::SA_ONSTACK);
        }

        // The initial thread of the new process shall inherit at
        // least the following attributes from the calling thread:
        //     Signal mask (see sigprocmask() and pthread_sigmask())
        //     Pending signals (see sigpending())
    }

    /// Fork the signal interface
    pub fn fork(&self) -> Self {
        Self {
            signal_actions: self.signal_actions,
            // The set of signals pending for the child process shall be
            // initialized to the empty set.
            signal_queue: VecDeque::new(),
            // The signal mask for a thread shall be initialized from that of its
            // parent or creating thread, or from the corresponding thread in the
            // parent process if the thread was created as the result of a call to
            // fork()
            current_sa_mask: self.current_sa_mask,
        }
    }

    /// Get a Job action(s) relative to signal_queue content
    /// This function is pure
    pub fn get_job_action(&self) -> JobAction {
        let mut action: JobAction = JobAction::default();
        let mut sa_mask = self.current_sa_mask;

        for &signum in self.signal_queue.iter() {
            if sa_mask.is_masked(signum) {
                continue;
            }
            let sigaction = self.signal_actions[signum];
            action |= match sigaction.sa_handler {
                SIG_IGN => JobAction::default(),
                SIG_DFL => {
                    use DefaultAction::*;
                    match signum.into() {
                        Abort | Terminate => JobAction::TERMINATE,
                        Stop => JobAction::STOP,
                        _ => JobAction::default(),
                    }
                }
                _ => {
                    sa_mask.update(signum, &sigaction);
                    JobAction::INTERRUPT
                }
            };
        }
        action
    }

    /// Create handler contexts and pop the signal queue. Return Some(signum) in case of Deadly signal
    pub fn exec_signal_handler(
        &mut self,
        cpu_state: *mut CpuState,
        user_stack_range: (u32, u32),
        in_blocked_syscall: bool,
    ) -> Option<Signum> {
        let mut i = 0;
        let mut frame_build = 0;

        while let Some(&signum) = self.signal_queue.get(i) {
            if self.current_sa_mask.is_masked(signum) {
                i += 1;
            } else {
                let sigaction = self.signal_actions[signum];
                match sigaction.sa_handler {
                    SIG_IGN => {}
                    SIG_DFL => match signum.into() {
                        DefaultAction::Abort | DefaultAction::Terminate => return Some(signum),
                        _ => {}
                    },
                    _ => {
                        let process_esp = unsafe { (*cpu_state).esp };
                        if process_esp >= user_stack_range.1 {
                            log::error!("ESP range of the current process is bullshit !");
                            log::error!(
                                "proc esp: {:#X?} > stack end: {:#X?}",
                                process_esp,
                                user_stack_range.1
                            );
                            return Some(Signum::SIGKILL);
                        }
                        // It There are not enough space in user stack (neg | x < PAGE_SIZE)
                        else if process_esp < user_stack_range.0
                            || process_esp - user_stack_range.0 < PAGE_SIZE as u32
                        {
                            log::warn!("ESP range underflow detected !");
                            log::warn!(
                                "proc esp: {:#X?}, stack start: {:#X?}",
                                process_esp,
                                user_stack_range.0
                            );
                            return Some(Signum::SIGKILL);
                        }
                        if frame_build == 0 && in_blocked_syscall {
                            if sigaction.sa_flags.intersects(SaFlags::SA_RESTART) {
                                // Back 2 instruction to reput eip on `int 80h` and restart the syscall
                                unsafe { (*cpu_state).eip -= 2 }
                            } else {
                                // Else the syscall must return Eintr
                                unsafe {
                                    (*cpu_state).registers.eax = (-(Errno::EINTR as i32)) as u32
                                };
                            }
                        }
                        frame_build += 1;
                        unsafe {
                            context_builder::push(
                                cpu_state,
                                self.current_sa_mask,
                                signum,
                                sigaction.sa_handler as u32,
                            );
                        }
                        self.current_sa_mask.update(signum, &sigaction);
                    }
                };
                self.signal_queue.remove(i);
            }
        }
        None
    }

    /// Acknowledge end of signal execution, pop the first internal signal and a restore context form the signal frame.
    pub fn terminate_pending_signal(&mut self, process_context_ptr: u32) {
        unsafe {
            self.current_sa_mask = context_builder::pop(process_context_ptr as *mut CpuState);
        }
    }

    /// Register a new handler for a specified Signum
    pub fn new_handler(
        &mut self,
        signum: Signum,
        sigaction: Option<&StructSigaction>,
    ) -> SysResult<StructSigaction> {
        // Associate a new action for a specified Signum
        match sigaction {
            None => Ok(self.signal_actions[signum]),
            Some(sigaction) => {
                // The system shall not allow the action for the signals SIGKILL or SIGSTOP to be set to SIG_IGN.
                if (signum == Signum::SIGKILL || signum == Signum::SIGSTOP)
                    && sigaction.sa_handler != SIG_DFL
                {
                    return Err(Errno::EINVAL);
                }

                // SIGCONT cannot be ignored (job control mandatory cf POSIX)
                if signum == Signum::SIGCONT && sigaction.sa_handler == SIG_IGN {
                    return Err(Errno::EINVAL);
                }
                Ok(mem::replace(&mut self.signal_actions[signum], *sigaction))
            }
        }
    }

    /// Register a new signal
    pub fn generate_signal(&mut self, signum: Signum) -> SysResult<u32> {
        // If the same signal already exists in signal queue, ignore it
        if self.signal_queue.iter().any(|&s| s == signum) {
            return Ok(0);
        }

        let default_action: DefaultAction = signum.into();

        // When any stop signal (SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU) is generated
        // for a process or thread, all pending SIGCONT signals for that process
        // or any of the threads within that process shall be
        // discarded.
        if default_action == DefaultAction::Stop {
            self.signal_queue.retain(|&s| s != Signum::SIGCONT);
        }

        // Conversely, when SIGCONT is generated for a process or
        // thread, all pending stop signals for that process or any of the
        // threads within that process shall be discarded
        if default_action == DefaultAction::Continue {
            self.signal_queue
                .retain(|&s| Into::<DefaultAction>::into(s) != DefaultAction::Stop);
        }

        self.signal_queue.try_reserve(1)?;
        self.signal_queue.push_back(signum);
        Ok(0)
    }

    pub fn change_signal_mask(
        &mut self,
        how: u32,
        set: Option<&sigset_t>,
        oldset: Option<&mut sigset_t>,
    ) -> SysResult<u32> {
        if let Some(set) = set {
            let mask = SaMask::from(*set);
            let current_sa_mask = self.current_sa_mask;
            let oldval = match how {
                SIG_BLOCK => mem::replace(&mut self.current_sa_mask, current_sa_mask | mask),
                SIG_UNBLOCK => mem::replace(&mut self.current_sa_mask, current_sa_mask & !mask),
                SIG_SETMASK => mem::replace(&mut self.current_sa_mask, mask),
                _ => {
                    return Err(Errno::EINVAL);
                }
            };
            if let Some(oldset) = oldset {
                *oldset = oldval.0;
            }
        } else {
            if let Some(oldset) = oldset {
                *oldset = self.current_sa_mask.0;
            }
        }
        Ok(0)
    }
}

/// This module allow to create contexts for handlers and to get back from them
mod context_builder {
    use super::{CpuState, SaMask, Signum};

    use core::mem::size_of;

    /// Create a new context witch will execute a signal handler
    pub unsafe fn push(
        cpu_state: *mut CpuState,
        sa_mask: SaMask,
        signum: Signum,
        handler_address: u32,
    ) {
        let mut user_esp = (*cpu_state).esp;

        /* PUSH DATA SECTION */

        // push the current cpu_state on the user stack
        push_esp(&mut user_esp, *cpu_state);

        // push the sa_mask
        push_esp(&mut user_esp, sa_mask);

        // push the trampoline code on the user stack
        push_buff_esp(
            &mut user_esp,
            symbol_addr!(_trampoline) as *mut u8,
            _trampoline_len as usize,
        );

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
