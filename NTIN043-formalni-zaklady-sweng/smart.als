
sig Home {}

assert noHome {
  no h: Home | h in Home
}

check noHome for 0