//! Ansi cursor move
use super::CSI;

use core::str::FromStr;
use core::{fmt, fmt::Display};

/// Usable to select write position for characters
#[derive(Debug, Copy, Clone, PartialEq, Default)]
pub struct Pos {
    /// Y coordinate
    pub line: usize,
    /// X coordinate
    pub column: usize,
}

/// Cursor possible moves
#[derive(Copy, Clone, Debug, PartialEq)]
pub enum CursorMove {
    ///Moves the cursor n (default 1) cells in the given direction. If the cursor is already at the edge of the screen, this has no effect.
    Up(usize),
    ///Rise an option if the cursor is on the bottom of the screen
    Down(usize),
    ///Rise an option if the cursor if on the bottom right if the screen
    Forward(usize),
    ///No special effect
    Backward(usize),

    ///Moves the cursor to column n
    HorizontalAbsolute(usize),

    ///Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted. A sequence such as CSI ;5H is a synonym for CSI 1;5H as well as CSI 17;H is the same as CSI 17H and CSI 17;1H
    Pos(Pos),
}

impl Display for CursorMove {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            CursorMove::Up(x) => write!(f, "{}{}A", CSI, x),
            CursorMove::Down(x) => write!(f, "{}{}B", CSI, x),
            CursorMove::Forward(x) => write!(f, "{}{}C", CSI, x),
            CursorMove::Backward(x) => write!(f, "{}{}D", CSI, x),
            CursorMove::HorizontalAbsolute(x) => write!(f, "{}{}G", CSI, x),
            CursorMove::Pos(Pos { line, column }) => write!(f, "{}{};{}H", CSI, line, column),
        }
    }
}

///Local error enum
#[derive(Debug)]
pub struct ParseCursorError;

impl FromStr for CursorMove {
    type Err = ParseCursorError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if s.len() < 4 || &s[0..=1] != CSI {
            return Err(ParseCursorError);
        }
        match &s[(s.len() - 1)..s.len()] {
            "H" => s.find(';').ok_or(ParseCursorError).and_then(|off| {
                let line: usize = s[2..off].parse().map_err(|_e| ParseCursorError)?;
                if off + 1 >= s.len() {
                    return Err(ParseCursorError);
                }
                let column: usize = s[off + 1..s.len() - 1]
                    .parse()
                    .map_err(|_e| ParseCursorError)?;
                Ok(CursorMove::Pos(Pos { line, column }))
            }),
            _ => {
                let nb: usize = s[2..s.len() - 1].parse().map_err(|_e| ParseCursorError)?;
                use CursorMove::*;
                Ok(match &s[(s.len() - 1)..s.len()] {
                    "A" => Up(nb),
                    "B" => Down(nb),
                    "C" => Forward(nb),
                    "D" => Backward(nb),
                    "G" => HorizontalAbsolute(nb),
                    _ => Err(ParseCursorError)?,
                })
            }
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_cursor() {
        use CursorMove::*;
        // println!("{}", Pos(terminal::Pos { line: 3, column: 3 }));
        // println!("{}", Pos(terminal::Pos { line: 1, column: 1 }));
        // println!("{}", Forward(10));

        let cursors = [
            Pos(crate::Pos {
                line: 1,
                column: 42,
            }),
            Up(10),
            Down(32),
            Forward(84),
            Backward(128),
            HorizontalAbsolute(16),
        ];

        for cursor in cursors.iter() {
            let cursor_str = &format!("{}", cursor);
            assert_eq!(CursorMove::from_str(cursor_str).unwrap(), *cursor);
        }
    }
}