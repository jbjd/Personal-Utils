use std::collections::HashMap;
use std::env;
use std::ffi::OsStr;
use std::fs::{self, DirEntry};
use std::hash::{DefaultHasher, Hash, Hasher};
use std::path::Path;

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

fn create_hash_to_file_map(
    dir: &Path,
    cb: &dyn Fn(&DirEntry) -> u64,
) -> Result<HashMap<u64, Vec<String>>, Box<dyn std::error::Error>> {
    let mut file_hashes: HashMap<u64, Vec<String>> = HashMap::new();

    if !dir.is_dir() {
        return Err(Box::from(format!("{:?} is not a directory", dir)));
    }

    let mut count: u32 = 0;
    for entry in fs::read_dir(dir)? {
        let entry: DirEntry = entry?;
        let path = entry.path();
        if path.is_dir() {
            // visit_dirs(&path, cb)?;
        } else {
            let file_hash: u64 = cb(&entry);
            if file_hash == 0 {
                continue;
            }
            let file_key: String = path.to_str().expect("Failed to get file name").to_owned();
            file_hashes
                .entry(file_hash)
                .or_insert_with(|| vec![])
                .push(file_key);

            count += 1;
            if count % 100 == 0 {
                println!("Loaded {} files", count);
            }
        }
    }

    Ok(file_hashes)
}

fn main() {
    let dir_buf = env::current_dir().unwrap();
    let dir = dir_buf.as_path();

    println!("Finding duplicates in {:?}\n", dir);
    let hashes_to_file = create_hash_to_file_map(dir, &generate_file_hash).unwrap();

    println!("\nPossible duplicates:");
    for (_, files) in hashes_to_file.iter() {
        if files.len() > 1 {
            println!("{:?}", files);
        }
    }
}
