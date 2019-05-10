type t;

type kind =
  | ZERO
  | INF
  | MINF
  | UNDEF
  | NZERO;

/* Bypass greatest common divisor */
external _fast_construct: ((Z.t, Z.t)) => t = "%identity";

[@bs.module "big-rat"] external of_ints: (int, int) => t = "default";
[@bs.module "big-rat"] external of_int: int => t = "default";
[@bs.module "big-rat"] external of_float: float => t = "default";
[@bs.module "big-rat"] external make: (Z.t, Z.t) => t = "default";
let of_bigint = n => _fast_construct((n, Z.one));

let one = of_int(1);
let zero = of_int(0);
let minus_one = of_int(-1);
let inf = of_ints(1, 0);
let minus_inf = of_ints(-1, 0);
let undef = of_ints(0, 0);

let of_string_base = (base: int, n: string) =>
  switch (StringUtil.stringSplitOnChar('/', n)) {
  | [num] => Z.of_string_base(base, num)->of_bigint
  | [num, den] =>
    make(Z.of_string_base(base, num), Z.of_string_base(base, den))
  | _ => undef
  };

let of_string = of_string_base(10);

[@bs.get_index] external num: (t, [@bs.as 0] _) => Z.t = "";
[@bs.get_index] external den: (t, [@bs.as 1] _) => Z.t = "";

[@bs.module "big-rat/add"] external add: (t, t) => t = "default";
[@bs.module "big-rat/sub"] external sub: (t, t) => t = "default";
[@bs.module "big-rat/mul"] external mul: (t, t) => t = "default";
[@bs.module "big-rat/div"] external div: (t, t) => t = "default";
[@bs.module "big-rat/neg"] external neg: t => t = "default";
[@bs.module "big-rat/abs"] external abs: t => t = "default";
[@bs.module "big-rat/recip"] external inv: t => t = "default";
[@bs.module "big-rat/to-string"] external to_string: t => string = "default";
[@bs.module "big-rat/to-float"] external to_float: t => float = "default";
[@bs.module "big-rat/cmp"] external compare: (t, t) => int = "default";
let equal = (a: t, b: t): bool => compare(a, b) == 0;
let lt = (a, b) => compare(a, b) == (-1);
let leq = (a, b) => compare(a, b) <= 0;
let gt = (a, b) => compare(a, b) == 1;
let geq = (a, b) => compare(a, b) >= 0;

let classify = n =>
  if (Z.equal(den(n), Z.zero)) {
    switch (Z.compare(num(n), Z.zero)) {
    | 1 => INF
    | (-1) => MINF
    | _ => UNDEF
    };
  } else {
    Z.equal(num(n), Z.zero) ? ZERO : NZERO;
  };

let (!=) = (a, b) => !equal(a, b);
let (==) = equal;
let (+) = add;
let (-) = sub;
let ( * ) = mul;
let (/) = div;
/* let (mod) = rem; */
let (~-) = neg;
let (<) = lt;
let (<=) = leq;
let (>) = gt;
let (>=) = geq;
