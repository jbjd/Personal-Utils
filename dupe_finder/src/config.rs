pub struct Flags;
impl Flags {
    pub const RECURSE: &str = "-r";
}

pub struct Config {
    pub recurse: bool,
}
impl Config {
    pub fn new() -> Self {
        Self { recurse: false }
    }
}
