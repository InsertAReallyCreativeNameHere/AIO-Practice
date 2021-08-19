
import turtle

def DrawPrism(l, w, d, color="lime"):
    tut = turtle.Screen()
    tut.bgcolor("white")
    tut.title("python is trash")

    pen = turtle.Turtle()
    pen.color(color)

    pen.penup()
    pen.goto(-l / 2, -w / 2)
    pen.pendown()

    pen.giveup()

def main():
    DrawPrism(0, 0, 0)

main()