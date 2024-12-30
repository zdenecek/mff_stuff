package h1
import scala.language.implicitConversions

case class Complex(real: Int, imaginary: Int):
  override def toString: String =
    s"$real${if imaginary < 0 then s"$imaginary" else s"+$imaginary"}i"

  def +(other: Complex): Complex =
    Complex(real + other.real, imaginary + other.imaginary)

  def -(other: Complex): Complex =
    Complex(real - other.real, imaginary - other.imaginary)

  def *(other: Complex): Complex =
    Complex(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real)

  def unary_- : Complex =
    Complex(-real, -imaginary)

object Complex:
  val I: Complex = Complex(0, 1)

  given Conversion[Int, Complex] with
    def apply(n: Int): Complex = Complex(n, 0)

import Complex.I


object ComplexNumbers:
	def main(args: Array[String]): Unit =
		println(Complex(1,2)) // 1+2i

		println(1 + 2*I + I*3 + 2) // 3+5i

		val c = (2+3*I + 1 + 4*I) * I
		println(-c) // 7-3i