use std::env;
use std::io::{self, BufRead};

struct DisjointSet {
    parent: Vec<usize>,
    rank: Vec<usize>,
}

impl DisjointSet {
    fn new(size: usize) -> Self {
        DisjointSet {
            parent: (0..size).collect(),
            rank: vec![0; size],
        }
    }

    fn find(&mut self, x: usize) -> usize {
        if self.parent[x] != x {
            self.parent[x] = self.find(self.parent[x]); // Path compression
        }
        self.parent[x]
    }

    fn merge(&mut self, x: usize, y: usize) {
        let root_x = self.find(x);
        let root_y = self.find(y);

        if root_x != root_y {
            if self.rank[root_x] > self.rank[root_y] {
                self.parent[root_y] = root_x;
            } else if self.rank[root_x] < self.rank[root_y] {
                self.parent[root_x] = root_y;
            } else {
                self.parent[root_y] = root_x;
                self.rank[root_x] += 1;
            }
        }
    }
}

pub fn main() {
    let args: Vec<String> = env::args().skip(1).collect();
    let mut ds: Option<DisjointSet> = None;

    let input = if args.is_empty() {
        let stdin = io::stdin();
        stdin.lock().lines().filter_map(Result::ok).collect::<Vec<String>>()
    } else {
        args
    };

    for command in input {
        let parts: Vec<&str> = command.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }

        match parts[0] {
            "init" => {
                if parts.len() != 2 {
                    eprintln!("Usage: init <size>");
                    continue;
                }
                if let Ok(size) = parts[1].parse::<usize>() {
                    ds = Some(DisjointSet::new(size));
                } else {
                    eprintln!("Invalid size");
                }
            }
            "merge" => {
                if let Some(ds) = ds.as_mut() {
                    if parts.len() != 3 {
                        eprintln!("Usage: merge <x> <y>");
                        continue;
                    }
                    if let (Ok(x), Ok(y)) = (parts[1].parse::<usize>(), parts[2].parse::<usize>()) {
                        ds.merge(x, y);
                    } else {
                        eprintln!("Invalid merge arguments");
                    }
                } else {
                    eprintln!("Disjoint set not initialized");
                }
            }
            "find" => {
                if let Some(ds) = ds.as_mut() {
                    if parts.len() != 2 {
                        eprintln!("Usage: find <x>");
                        continue;
                    }
                    if let Ok(x) = parts[1].parse::<usize>() {
                        println!("{}", ds.find(x));
                    } else {
                        eprintln!("Invalid find argument");
                    }
                } else {
                    eprintln!("Disjoint set not initialized");
                }
            }
            _ => {
                eprintln!("Unknown command: {}", parts[0]);
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_disjoint_set() {
        let mut ds = DisjointSet::new(5);
        
        // Initial state: each element is its own parent
        assert_eq!(ds.find(0), 0);
        assert_eq!(ds.find(1), 1);
        assert_eq!(ds.find(2), 2);
        assert_eq!(ds.find(3), 3);
        assert_eq!(ds.find(4), 4);

        // Merge 0 and 2
        ds.merge(0, 2);
        assert_eq!(ds.find(0), ds.find(2));

        // Merge 1 and 3
        ds.merge(1, 3);
        assert_eq!(ds.find(1), ds.find(3));

        // Find representatives
        assert_eq!(ds.find(0), 0);
        assert_eq!(ds.find(2), 0);
        assert_eq!(ds.find(1), 1);
        assert_eq!(ds.find(3), 1);

        // Merge 0 and 1
        ds.merge(0, 1);
        assert_eq!(ds.find(2), ds.find(3));
        assert_eq!(ds.find(0), ds.find(1));
    }

    #[test]
    fn test_disjoint_set_chain() {
        let mut ds = DisjointSet::new(3);
        
        // Merge all elements
        ds.merge(0, 1);
        ds.merge(1, 2);

        // All elements should have the same representative
        assert_eq!(ds.find(0), ds.find(1));
        assert_eq!(ds.find(1), ds.find(2));
        assert_eq!(ds.find(0), ds.find(2));
    }
}
