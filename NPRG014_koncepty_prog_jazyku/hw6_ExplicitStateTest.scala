/*
Implement the classes below such that the main (without modifications) prints out the something like this:

Person John Doe aged 24
Person John Doe aged 25
List(h2.PersonState@3d24753a)
Person John Doe aged 24
Thing Box with color (255,0,0)
Person Joe aged 24

*/

package h2

import scala.collection.mutable.ListBuffer


trait WithExplicitState:
  /* add necessary declarations here */
  type StateType
  protected def state: StateType/* add the correct type here */
  protected def state_=(state: StateType/* add the correct type here */): Unit


class PersonState(val name: String, val age: Int)

class Person extends WithExplicitState:
  /* Implement this class. It should have no knowledge of the trait History. It should use instances of PersonState as the state. */
  type StateType = PersonState
  private var _state: PersonState = new PersonState("", 0)
  
  protected def state: PersonState = _state
  protected def state_=(s: PersonState): Unit = _state = s
  
  def setName(name: String): this.type =
    state = new PersonState(name, state.age)
    this
  
  def setAge(age: Int): this.type =
    state = new PersonState(state.name, age)
    this
  
  override def toString: String =
    s"Person ${state.name} aged ${state.age}"


type RGBColor = (Int, Int, Int)
class ThingState(val name: String, val color: RGBColor)

class Thing extends WithExplicitState:
  /* Implement this class. It should have no knowledge of the trait History. It should use instances of ThingState as the state. */
  type StateType = ThingState
  private var _state: ThingState = new ThingState("", (0, 0, 0))
  
  protected def state: ThingState = _state
  protected def state_=(s: ThingState): Unit = _state = s
  
  def setName(name: String): this.type =
    state = new ThingState(name, state.color)
    this
  
  def setColor(color: RGBColor): this.type =
    state = new ThingState(state.name, color)
    this
  
  override def toString: String =
    s"Thing ${state.name} with color ${state.color}"


trait History  extends WithExplicitState:
    /* Add necessary declarations here. This trait should have no knowledge of classes Person, Thing, PersonState, ThingState.
       It should depend only on the trait WithExplicitState.
    */

    val hist = ListBuffer.empty[StateType/* add the correct type here */]

    def checkpoint(): this.type/* add the correct type here */ =
      hist.append(state)
      this

    def history = hist.toList

    def restoreTo(s: StateType/* add the correct type here */): this.type/* add the correct type here */ =
      state = s
      this


object ExplicitStateTest:
  def main(args: Array[String]): Unit =
    // The inferred type of variable "john" should be "Person & History".
    val john = (new Person with History).setName("John Doe").setAge(24).checkpoint()

    println(john)
    john.setAge(25)

    println(john)
    println(john.history)

    val johnsPrevState = john.history(0)
    john.restoreTo(johnsPrevState)
    println(john)

    // The inferred type of variable "box" should be "Thing & History".
    val box = new Thing with History
    box.setName("Box")
    box.setColor((255, 0, 0))
    println(box)

    val joe = new Person with History
    joe.restoreTo(johnsPrevState).setName("Joe")
    println(joe)

    // The line below must not compile. It should complain about an incompatible type.
    // box.restoreTo(johnsPrevState)
    // does not compile
