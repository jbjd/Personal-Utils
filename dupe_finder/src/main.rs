use std::collections::HashMap;
use std::ffi::OsStr;
use std::fs::{self, DirEntry};
use std::hash::{DefaultHasher, Hash, Hasher};
use std::path::Path;
use std::{env, io};

use config::{Config, Flags};

mod config;

fn generate_file_hash(dir_entry: &DirEntry) -> u64 {
    let path = dir_entry.path();
    let file_name = path.file_name().unwrap_or(OsStr::new(""));

    let file_contents = match fs::read(dir_entry.path()) {
        Ok(res) => res,
        Err(e) => {
            eprintln!("Error creating file hash for file {:?}: {:?}", file_name, e);
            return 0;
        }
    };

    if file_contents.len() == 0 {
        println!("Skipping empty file {:?}", file_name);
        return 0;
    }

    let mut hasher = DefaultHasher::new();
    file_contents.hash(&mut hasher);
    hasher.finish()
}

fn populate_hashes_to_files(
    dir: &Path,
    cb: &dyn Fn(&DirEntry) -> u64,
    config: &Config,
    file_hashes: &mut HashMap<u64, Vec<String>>,
    count: &mut u32,
) -> Result<(), io::Error> {
    if !dir.is_dir() {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            format!("{:?} is not a directory", dir),
        ));
    }

    for entry in fs::read_dir(dir)? {
        let dir_entry: DirEntry = entry?;
        let path = dir_entry.path();
        if path.is_dir() {
            if config.recurse {
                populate_hashes_to_files(&path, cb, config, file_hashes, count)?;
            }
        } else {
            let file_hash: u64 = cb(&dir_entry);
            if file_hash == 0 {
                continue;
            }
            let file_path: String = path.to_str().expect("Failed to get file name").to_owned();
            file_hashes
                .entry(file_hash)
                .or_insert_with(|| vec![])
                .push(file_path);

            *count += 1;
            if *count % 100 == 0 {
                println!("Loaded {} files", count);
            }
        }
    }

    Ok(())
}

fn get_hashes_to_files(
    dir: &Path,
    cb: &dyn Fn(&DirEntry) -> u64,
    config: Config,
) -> Result<HashMap<u64, Vec<String>>, io::Error> {
    let mut file_hashes: HashMap<u64, Vec<String>> = HashMap::new();
    let mut count: u32 = 0;

    let result = populate_hashes_to_files(dir, &cb, &config, &mut file_hashes, &mut count);

    match result {
        Ok(_) => return Ok(file_hashes),
        Err(e) => return Err(e),
    }
}

fn get_config() -> Config {
    let mut config = Config::new();
    let args: Vec<String> = env::args().collect();

    if args.contains(&Flags::RECURSE.to_string()) {
        config.recurse = true;
    }

    config
}

fn main() {
    let config = get_config();
    let dir_buf = env::current_dir().unwrap();
    let dir = dir_buf.as_path();

    println!("Finding duplicates in {:?}\n", dir);
    let hashes_to_file = get_hashes_to_files(dir, &generate_file_hash, config).unwrap();

    println!("\nPossible duplicates:");
    for (_, files) in hashes_to_file.iter() {
        if files.len() > 1 {
            println!("{:?}", files);
        }
    }
}
