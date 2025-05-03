
// TASK 1
// Zdenek Tomis

open util/ordering[SystemState] as ord

abstract sig Device {}
sig Light extends Device {}
sig Heater extends Device {}
sig Camera extends Device {}
sig Lock extends Device {}
sig Thermostat extends Device {}
sig Blind extends Device {}
sig Sprinkler extends Device {}

abstract sig DeviceState {}
one sig On, Off, Locked, Unlocked, Open, Closed extends DeviceState {}

abstract sig SensorState {}
one sig Smoke, NoSmoke, Motion, NoMotion, LowHumidity, HighHumidity, LowBattery, SufficientBattery extends SensorState {}

sig Sensor extends Device {
    smoke: one SensorState,
    motion: one SensorState,
    temperature: Int,
    humidity: one SensorState,
    battery: one SensorState
} { 
    smoke in Smoke + NoSmoke
    motion in Motion + NoMotion
    humidity in LowHumidity + HighHumidity
    battery in LowBattery + SufficientBattery
 }

sig SmartHome {
    devices: set Device,
    sensors: set Sensor,
    users: set User
}

sig SystemState {
    smartHome: one SmartHome,
    deviceState: Device -> one DeviceState
} { 
    all l: Lock | deviceState[l] in Locked + Unlocked + Off
  	all d: Light | deviceState[d] in On + Off
	all d: Heater | deviceState[d] in On + Off
	all d: Camera | deviceState[d] in On + Off
	all d: Thermostat | deviceState[d] in On + Off
	all d: Blind | deviceState[d] in Open + Closed + Off
	all d: Sprinkler | deviceState[d] in On + Off 
}



sig User {
    access: set Lock,
    permissions: set Light + Blind + Thermostat
} {
 all d: Light + Blind + Thermostat | some sh: SmartHome  | d in permissions and this in sh.users implies  d in sh.devices
all d: Lock | some sh: SmartHome  | d in access and this in sh.users implies  d in sh.devices
}


fact {
  all sh: SmartHome | some ss: SystemState | ss.smartHome = sh
  all d: Device | some sh: SmartHome | d in sh.devices
  all u: User | some sh: SmartHome | u in sh.users
}

fact initialState {
    let firstState = ord/first |
        all d: firstState.smartHome.devices | d->Off in firstState.deviceState
}

pred LightOnSmoke [prev: SystemState, next: SystemState] {
    all l: Light, s: Sensor |
        (s.smoke = Smoke) implies l->On in next.deviceState
}

pred CameraOnMotion [prev: SystemState, next: SystemState] {
    all c: Camera, s: Sensor |
        (s.motion = Motion) implies c->On in next.deviceState
}

pred UnlockOnAccess [prev: SystemState, next: SystemState] {
    all k: Lock, u: User |
        k in u.access and k->Locked in prev.deviceState implies k->Unlocked in next.deviceState
}

pred HeaterOnLowTemp [prev: SystemState, next: SystemState] {
    all h: Heater, s: Sensor |
        (s.temperature < 18) implies h->On in next.deviceState
}

pred HeaterOffHighTemp [prev: SystemState, next: SystemState] {
    all h: Heater, s: Sensor |
        (s.temperature > 22) implies h->Off in next.deviceState
}

pred BlindOpenOnMotion [prev: SystemState, next: SystemState] {
    all b: Blind, s: Sensor |
        (s.motion = Motion) implies b->Open in next.deviceState
}

pred ThermostatControlled [prev: SystemState, next: SystemState] {
    all t: Thermostat, u: User |
        t in u.permissions implies t->On in next.deviceState or t->Off in next.deviceState
}

pred SprinklerOnLowHumidity [prev: SystemState, next: SystemState] {
    all sp: Sprinkler, s: Sensor |
        (s.humidity = LowHumidity) implies sp->On in next.deviceState
}

pred DevicesOffOnLowBattery [prev: SystemState, next: SystemState] {
    all s: Sensor |
        s.battery = LowBattery implies
        all d: Device | d->Off in next.deviceState
}

pred AddUser [prev: SystemState, next: SystemState] {
    next.smartHome.users = prev.smartHome.users + User
}

pred RemoveUser [prev: SystemState, next: SystemState] {
    some u: prev.smartHome.users | next.smartHome.users = prev.smartHome.users - u
}

pred AddDeviceOrSensor [prev: SystemState, next: SystemState] {
    next.smartHome.devices = prev.smartHome.devices + Device or
    next.smartHome.sensors = prev.smartHome.sensors + Sensor
}

pred RemoveDevice [prev: SystemState, next: SystemState] {
    some d: prev.smartHome.devices | next.smartHome.devices = prev.smartHome.devices - d
}

pred AtMostOneChange [prev: SystemState, next: SystemState] {
    (AddUser[prev, next] and not (RemoveUser[prev, next] or AddDeviceOrSensor[prev, next] or RemoveDevice[prev, next])) or
    (RemoveUser[prev, next] and not (AddUser[prev, next] or AddDeviceOrSensor[prev, next] or RemoveDevice[prev, next])) or
    (AddDeviceOrSensor[prev, next] and not (AddUser[prev, next] or RemoveUser[prev, next] or RemoveDevice[prev, next])) or
    (RemoveDevice[prev, next] and not (AddUser[prev, next] or RemoveUser[prev, next] or AddDeviceOrSensor[prev, next]))
}

pred ValidTransition [prev: SystemState, next: SystemState] {
    (LightOnSmoke[prev, next] and
    CameraOnMotion[prev, next] and
    UnlockOnAccess[prev, next] and
    HeaterOnLowTemp[prev, next] and
    HeaterOffHighTemp[prev, next] and
    BlindOpenOnMotion[prev, next] and
    ThermostatControlled[prev, next] and
    SprinklerOnLowHumidity[prev, next] and
    DevicesOffOnLowBattery[prev, next]) and
    AtMostOneChange[prev, next]
}

assert UserCanAlwaysUnlock {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all k: Lock, u: User |
            k in u.access and k->Locked in prev.deviceState implies k->Unlocked in next.deviceState
}

assert SmokeTriggersLights {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all s: Sensor, l: Light |
            s.smoke = Smoke implies l->On in next.deviceState
}

assert MotionTriggersCameras {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all s: Sensor, c: Camera |
            s.motion = Motion implies c->On in next.deviceState
}

assert TemperatureTriggersHeater {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all s: Sensor, h: Heater |
            (s.temperature < 18 implies h->On in next.deviceState) and
            (s.temperature > 22 implies h->Off in next.deviceState)
}

assert HumidityTriggersSprinkler {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all s: Sensor, sp: Sprinkler |
            s.humidity = LowHumidity implies sp->On in next.deviceState
}

assert BlindsOpenOnMotion {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all s: Sensor, b: Blind |
            s.motion = Motion implies b->Open in next.deviceState
}

assert AddingDeviceDoesNotTriggerSprinkler {
    all prev, next: SystemState |
        ValidTransition[prev, next] implies
        all s: Sensor, sp: Sprinkler |
        no other_s: Sensor  |
            other_s in prev.smartHome.sensors and  other_s.humidity = LowHumidity and
            not(s in prev.smartHome.sensors) and s in next.smartHome.sensors and
            sp in prev.smartHome.devices and sp->Off in prev.deviceState
             implies sp->Off in next.deviceState
}

run { some SystemState } for exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User, exactly 10 SystemState

check UserCanAlwaysUnlock for 5 SystemState, exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User expect 0
check SmokeTriggersLights for 5 SystemState, exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User expect 0
check MotionTriggersCameras for 5 SystemState,exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User expect 0
check TemperatureTriggersHeater for 5 SystemState, exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User expect 0
check HumidityTriggersSprinkler for 5 SystemState, exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User  expect 0
check BlindsOpenOnMotion for 5 SystemState, exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User expect 0

// example of failing assertion

check AddingDeviceDoesNotTriggerSprinkler for 5 SystemState, exactly 5 Device, 3 SmartHome, 
exactly 3 Sensor, exactly 2 User expect 0


// OUTPUT:
//    #1: .run$1 is consistent.
//   #2: No counterexample found. UserCanAlwaysUnlock may be valid, as expected.
//   #3: No counterexample found. SmokeTriggersLights may be valid, as expected.
//   #4: No counterexample found. MotionTriggersCameras may be valid, as expected.
//   #5: No counterexample found. TemperatureTriggersHeater may be valid, as expected.
//   #6: No counterexample found. HumidityTriggersSprinkler may be valid, as expected.
//   #7: No counterexample found. BlindsOpenOnMotion may be valid, as expected.
//   #8: .AddingDeviceDoesNotTriggerSprinkler is invalid, contrary to expectation.




