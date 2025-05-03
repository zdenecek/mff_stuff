

use std::convert::{TryFrom, TryInto};

#[derive(Debug, PartialEq)]
pub enum Error {
    InvalidMove,
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum PieceType {
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    Pawn,
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Piece {
    White(PieceType),
    Black(PieceType),
}

impl Piece {
    pub fn as_turn(&self) -> Turn {
        match self {
            Piece::White(_) => Turn::WhitePlays,
            Piece::Black(_) => Turn::BlackPlays,
        }
    }

}

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Turn {
    WhitePlays,
    BlackPlays,
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Position {
    pub x: u8,
    pub y: u8,
}



impl TryFrom<&str> for Position {
    type Error = Error;

    fn try_from(string: &str) -> Result<Self, Self::Error> {
        let x = match string.chars().nth(0) {
            Some('a') => 0,
            Some('b') => 1,
            Some('c') => 2,
            Some('d') => 3,
            Some('e') => 4,
            Some('f') => 5,
            Some('g') => 6,
            Some('h') => 7,
            _ => return Err(Error::InvalidMove),
        };

        let y = match string.chars().nth(1) {
            Some('1') => 0,
            Some('2') => 1,
            Some('3') => 2,
            Some('4') => 3,
            Some('5') => 4,
            Some('6') => 5,
            Some('7') => 6,
            Some('8') => 7,
            _ => return Err(Error::InvalidMove),
        };

        Ok(Position { x, y })
    }
}

impl TryFrom<(u8, u8)> for Position {
    type Error = Error;

    fn try_from(coordinates: (u8, u8)) -> Result<Self, Self::Error> {
        let (x, y) = coordinates;
        if x > 7 || y > 7 {
            return Err(Error::InvalidMove);
        }

        Ok(Position { x, y })
    }
}

impl TryInto<Position> for String {
    type Error = Error;

    fn try_into(self) -> Result<Position, Self::Error> {
        Position::try_from(self.as_str())
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct ChessGame {
    board: [[Option<Piece>; 8]; 8],
    turn: Turn,
}

impl ChessGame {

    pub fn current_player(self) -> Turn {
        self.turn
    }

    pub fn new_game() -> ChessGame {
        let board = [
            [
                Some(Piece::White(PieceType::Rook)),
                Some(Piece::White(PieceType::Knight)),
                Some(Piece::White(PieceType::Bishop)),
                Some(Piece::White(PieceType::Queen)),
                Some(Piece::White(PieceType::King)),
                Some(Piece::White(PieceType::Bishop)),
                Some(Piece::White(PieceType::Knight)),
                Some(Piece::White(PieceType::Rook)),
            ],
            
            [
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
                Some(Piece::White(PieceType::Pawn)),
            ],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
                Some(Piece::Black(PieceType::Pawn)),
            ],
            [
                Some(Piece::Black(PieceType::Rook)),
                Some(Piece::Black(PieceType::Knight)),
                Some(Piece::Black(PieceType::Bishop)),
                Some(Piece::Black(PieceType::Queen)),
                Some(Piece::Black(PieceType::King)),
                Some(Piece::Black(PieceType::Bishop)),
                Some(Piece::Black(PieceType::Knight)),
                Some(Piece::Black(PieceType::Rook)),
            ],
        ];

        ChessGame {
            board,
            turn: Turn::WhitePlays,
        }
    }

    pub fn make_move(&mut self, src: Position, dst: Position) -> Result<Option<Piece>, Error> {
        let piece = self.get_field(src);
        let captured_piece = self.get_field(dst);
    
        if !self.is_valid_move(src, dst) {
            return Err(Error::InvalidMove);
        }
    
        self.set_field(dst, piece);
        self.set_field(src, None);

        self.turn = match self.turn {
            Turn::WhitePlays => Turn::BlackPlays,
            Turn::BlackPlays => Turn::WhitePlays,
        };
    
        Ok(captured_piece)
    }

    
    pub fn is_valid_move(&self, from: Position, to: Position) -> bool {
            // Check that the "from" and "to" positions are on the board
            if from.x > 7 || from.y > 7 || to.x > 7 || to.y > 7 {
                return false;
            }
    
            // Check that the "from" position contains a piece
            let piece = match self.board[usize::from(from.y)][usize::from(from.x)] {
                Some(p) => p,
                None => return false,
            };
    
            // Check that the piece belongs to the current player
            if piece.as_turn() != self.turn {
                return false;
            }

            match self.get_field(to) {
                Some(p) => return p.as_turn() != self.turn ,
                None => return true,
            }
        }
    

    fn set_field(&mut self, pos: Position, piece: Option<Piece>) {
        self.board[usize::from(pos.y)][usize::from(pos.x)] = piece;
    }

    pub fn get_field(&self, pos: Position) -> Option<Piece> {
        self.board[usize::from(pos.y)][usize::from(pos.x)]
    }
}