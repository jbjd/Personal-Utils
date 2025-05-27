pub enum Flag {
    ShortHelp,
    LongHelp,
}

impl Flag {
    pub fn value(&self) -> String {
        match *self {
            Flag::ShortHelp => "-h",
            Flag::LongHelp => "--help",
        }
        .to_owned()
    }
}
