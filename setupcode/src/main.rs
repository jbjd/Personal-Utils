use std::env;
use std::path::{Path, PathBuf};
use std::process;

mod flags;
use flags::Flag;

// TODO: Add more!
const VALID_ARGS: [&str; 1] = ["python"];

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2
        || args.contains(&Flag::ShortHelp.value())
        || args.contains(&Flag::LongHelp.value())
    {
        print_help();
        process::exit(0);
    }

    let choosen_language = &args[1];
    if !VALID_ARGS.contains(&choosen_language.as_str()) {
        eprintln!("Invalid language \"{}\" choosen\n", choosen_language);
        print_accepted_languages();
        process::exit(1);
    }

    let current_dir: PathBuf = env::current_dir().unwrap();
    let current_exe: PathBuf = env::current_exe().unwrap();
    let exe_dir = current_exe
        .parent()
        .unwrap()
        .join("setup")
        .join(choosen_language);
    println!("Copying {:?} to {:?}", exe_dir, current_dir);
}

fn print_help() {
    println!("Sets up an empty directory for a given coding language");
    println!("Called like: `setupcode [language]`\n");
    print_accepted_languages();
}

fn print_accepted_languages() {
    println!("Accepted languages include: {:?}", VALID_ARGS)
}
