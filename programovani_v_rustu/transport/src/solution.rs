use core::panic;
use std::cell::RefCell;
use std::collections::{HashMap, HashSet};
use std::rc::Rc;

// use std::fmt::Display;

macro_rules! rcbox {
    ($stuff:tt) => {
        Rc::new(Box::new($stuff))
    };
}

macro_rules! refcellbox {
    ($stuff:tt) => {
        RefCell::new(Box::new($stuff))
    };
}

fn road_identifier(c1: &Rc<Box<City>>, c2: &Rc<Box<City>>) -> (Rc<Box<City>>, Rc<Box<City>>) {
    if c1 < c2 {
        (c1.clone(), c2.clone())
    } else {
        (c2.clone(), c1.clone())
    }
}

#[derive(Debug, Hash, PartialEq, Eq, PartialOrd, Ord, Clone)]
pub struct City {
    name: String,
}

impl City {
    pub fn name(&self) -> String {
        self.name.clone()
    }
    pub fn new(name: &str) -> Self {
        City {
            name: name.to_string(),
        }
    }
}

#[derive(Debug, PartialEq)]
pub struct Road {
    from: Rc<Box<City>>,
    to: Rc<Box<City>>,
    value: i32,
}

impl Road {
    pub fn new(from: &Rc<Box<City>>, to: &Rc<Box<City>>, value: i32) -> Road {
        Road {
            from: from.clone(),
            to: to.clone(),
            value,
        }
    }
}

#[derive(Debug, Clone)]
pub struct Bus {
    cities: Vec<Rc<Box<City>>>,
    flocks: Vec<Rc<Box<Flocks>>>,
    current_stop: Option<Rc<Box<City>>>,
    next_stop: Option<Rc<Box<City>>>,
    counter: usize,
    distance_left: i32,
    arrived_at_end: bool,
}

impl Bus {
    pub fn new(start: &Rc<Box<City>>, cities: Vec<Rc<Box<City>>>) -> Bus {
        Bus {
            cities: cities,
            flocks: Vec::new(),
            current_stop: None,
            next_stop: Some(start.clone()),
            counter: 0,
            distance_left: 1,
            arrived_at_end: false,
        }
    }
}

#[derive(Clone, Debug)]
pub struct Event {
    city: Rc<Box<City>>,
    on: u32,
    off: u32,
}

impl Event {
    pub fn city(&self) -> Rc<Box<City>> {
        self.city.clone()
    }
    pub fn got_on(&self) -> u32 {
        self.on
    }
    pub fn got_off(&self) -> u32 {
        self.off
    }
}

#[derive(Debug)]
pub struct Simulation {
    cities: HashSet<Rc<Box<City>>>,
    roads: HashMap<(Rc<Box<City>>, Rc<Box<City>>), i32>,
    buses: Vec<RefCell<Box<Bus>>>,
    flocks: Vec<Rc<Box<Flocks>>>,
    time: u32,
}

#[derive(Debug, Clone)]
struct Flocks {
    from: Rc<Box<City>>,
    to: Rc<Box<City>>,
    count: u32,
}

impl Simulation {
    pub fn new() -> Self {
        Simulation {
            cities: HashSet::new(),
            roads: HashMap::new(),
            flocks: Vec::new(),
            buses: Vec::new(),
            time: 0,
        }
    }

    pub fn new_city(&mut self, name: &str) -> Rc<Box<City>> {
        let city = City::new(name);
        let city = rcbox!(city);
        self.cities.insert(city.clone());
        city
    }

    pub fn new_road(
        &mut self,
        from: &Rc<Box<City>>,
        to: &Rc<Box<City>>,
        distance: i32,
    ) -> Rc<Box<Road>> {
        if !self.cities.contains(from) || !self.cities.contains(to) {
            panic!("Invalid city name.");
        }

        let (from, to) = road_identifier(from, to);
        let road = Road::new(&from, &to, distance);
        let road = rcbox!(road);

        self.roads.insert((from, to), distance);

        road
    }

    pub fn new_bus(&mut self, cities: &[&Rc<Box<City>>]) {
        if cities.len() <= 1 {
            panic!("Invalid bus route.");
        }
        // check all adjacent pairs for a road
        for index in 1..cities.len() {
            let a = *cities.get(index).unwrap();
            let b = *cities.get(index - 1).unwrap();

            let (c1, c2) = road_identifier(a, b);

            if !self.roads.contains_key(&(c1, c2)) {
                panic!("Cities not connected.");
            }
        }

        let start = cities.first().unwrap().clone();
        let cities = cities.iter().cloned().cloned().collect();
        let bus = Bus::new(start, cities);
        self.buses.push(refcellbox!(bus));
    }

    pub fn add_people(&mut self, from: &Rc<Box<City>>, to: &Rc<Box<City>>, count: u32) -> () {
        let flocks = Flocks {
            from: from.clone(),
            to: to.clone(),
            count: count,
        };
        let flocks = rcbox!(flocks);
        self.flocks.push(flocks);
    }

    fn step_simulation(&mut self) -> Vec<Event> {
        let mut current_events = Vec::new();
        self.time += 1;

        for bus in  &mut self.buses {
            if bus.borrow().arrived_at_end {
                continue;
            }

            let mut bus = bus.borrow_mut();
            bus.distance_left -= 1;

            let mut event = Box::new(Event {
                city: bus.next_stop.clone().unwrap(),
                off: 0,
                on: 0,
            });
            
            // add all the people that get on at this stop
            event.on = {
                let mut count: u32 = 0;
                let mut flocks = Vec::new();
                for flock in self.flocks.iter() {
                    let city = bus.next_stop.clone().unwrap();
                    let next_cities: Vec<&Rc<Box<City>>> = bus
                        .cities
                        .iter()
                        .rev()
                        .take(bus.cities.len() - bus.counter)
                        .collect();
                    if bus.distance_left == 0 && city == flock.from && next_cities.contains(&&flock.to)
                    {
                        bus.flocks.push(flock.clone());
                        count += flock.count;
                    } else {
                        flocks.push(flock.clone());
                    }
                }
                self.flocks = flocks;
                count
            };

            // remove all the people that get off at this stop
            event.off =  {
                let mut count: u32 = 0;
                if bus.distance_left <= 0 {
                    let mut flocks = Vec::new();
                    for flock in bus.flocks.iter() {
                        if flock.to == bus.next_stop.clone().unwrap() {
                            count += flock.count;
                        } else {
                            flocks.push(flock.clone());
                        }
                    }
                    bus.flocks = flocks;
                }
                count
            };

            if  bus.distance_left == 0 {
                current_events.push(*event);
            }

            if bus.next_stop.clone().unwrap() != *bus.cities.last().unwrap()
                && bus.distance_left == 0
            {
                bus.current_stop = bus.next_stop.clone();
                bus.counter += 1;

                let next = bus.cities.iter().nth(bus.counter as usize);

                let (dist, next) = match next {
                    Some(stop) => {
                        let dist = *self
                            .roads
                            .get(&road_identifier(&bus.current_stop.clone().unwrap(), &stop))
                            .unwrap()
                            ;

                        (dist,  Some(stop.clone()))
                    }
                    _ => {
                        
                        bus.arrived_at_end = true;
                        (-1, None)
                    }
                };

                bus.distance_left = dist;
                bus.next_stop = next;
            }
        }
        current_events
    }

    pub fn execute(&mut self, max_time: i32) -> Vec<Event> {
        let mut events = Vec::new();

        for _ in 0..max_time {
            let ev = self.step_simulation();
            events.extend(ev);
        }

        events
    }

}
