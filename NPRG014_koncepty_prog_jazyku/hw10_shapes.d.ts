export type NumericColor = {
  r: number;
  g: number;
  b: number;
};

type StringColor = "red" | "green" | "blue";

export type Color = StringColor | NumericColor;

type ShapeType = "circle" | "square" | "rectangle" | "triangle";

export interface Circle {
  type: "circle";
  radius: number;
  color: Color;
}

export interface Square {
  type: "square";
  width: number;
  color: Color;
}

export interface Rectangle {
  type: "rectangle";
  width: number;
  height: number;
  color: Color;
}

export interface Triangle {
  type: "triangle";
  base: number;
  height: number;
  color: Color;
}

export type Shape = Circle | Square | Rectangle | Triangle;

// Function to create shapes
export declare function createCircle(
  radius: number,
  color: Color | NumericColor
): Circle;
export declare function createSquare(
  width: number,
  color: Color | NumericColor
): Square;
export declare function createRectangle(
  width: number,
  height: number,
  color: Color | NumericColor
): Rectangle;
export declare function createTriangle(
  base: number,
  height: number,
  color: Color | NumericColor
): Triangle;

// Shape factory function
export declare function createShape(
  type: "circle",
  radius: number,
  color: Color | NumericColor
): Circle;

export declare function createShape(
  type: "square",
  width: number,
  color: Color | NumericColor
): Square;

export declare function createShape(
  type: "rectangle",
  width: number,
  height: number,
  color: Color | NumericColor
): Rectangle;

export declare function createShape(
  type: "triangle",
  base: number,
  height: number,
  color: Color | NumericColor
): Triangle;

export declare function calculateShapeArea(
  shape: Circle | Rectangle | Triangle
): number;

export const colorTable: Record<StringColor, NumericColor>;

export declare function invertColor<T extends { color: Color | NumericColor }>(
  shape: T
): T & { color: NumericColor };

export declare function filterShapesByType<S extends Shape, T extends ShapeType>(
  shapes: S[],
  types: T[]
): Extract<S, { type: T }>[];

type ObserverCallback<T, K extends keyof T> = (
  oldValue: T[K],
  newValue: T[K]
) => void;

export type ObservableShape<T extends Shape> = T & {
  on<K extends keyof T>(eventName: K, callback: ObserverCallback<T, K>): void;
  update(changes: Partial<T>): void;
};

export declare function makeObservable<T extends Shape>(shape: T): ObservableShape<T>;
