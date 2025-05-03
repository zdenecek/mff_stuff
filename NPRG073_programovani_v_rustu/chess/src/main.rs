


use solution::ChessGame;
use solution::Error;


fn main() {
    
    test().expect("failed");
    
}

fn test() -> Result<(), Error> {
    let mut game = ChessGame::new_game();


    let pos1 = "a2".try_into()?;
    let pos2 = "a3".try_into()?;
    let pos3 = "g8".try_into()?;
    let pos4 = "a3".try_into()?;

    println!("caught {:?}", game.make_move(pos1, pos2)?);
    println!("caught {:?}", game.make_move(pos3, pos4)?);

    Ok(())
}