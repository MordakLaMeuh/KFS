use messaging::MessageTo;

use lazy_static::lazy_static;
use sync::LockForest;

/// message queue can contain 5 messages
const MESSAGE_QUEUE_CAPACITY: usize = 5;

lazy_static! {
    /// Global Message queue of the kernel, Written by anybody and
    /// readen by the scheduler which can dispatch the message afterwards
    static ref MESSAGE_QUEUE: LockForest<MessageTo> = LockForest::new(MESSAGE_QUEUE_CAPACITY);
}

/// push a message on to the global MESSAGE_QUEUE
pub fn push_message(message: MessageTo) {
    MESSAGE_QUEUE.push(message).unwrap_or_else(|_| {
        log::error!("Message queue is full");
    });
}

pub fn drain_messages() -> impl Iterator<Item = MessageTo> {
    MESSAGE_QUEUE.drain()
}
