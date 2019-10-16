open Types;

let (~-) = Base_Functions.neg;
let (+) = Base_Operators.add;
let (-) = Base_Operators.sub;
let ( * ) = Base_Operators.mul;
let (/) = Base_Operators.div;
let (==) = Base_Comparison.equal;
let exp = Base_Exponentiation.exp;
let log = Base_Exponentiation.log;

let qTwo = Q.of_int(2);
let qHalf = Q.of_ints(1, 2);
let half = `Real((qHalf, Constant.Unit));

let zeroS = `Real((Q.zero, Constant.Unit));
let oneS = `Real((Q.one, Constant.Unit));
let toIdentity = m =>
  `Matrix(
    Matrix.mapWithIndex(m, (row, column, _) =>
      Pervasives.(row == column) ? oneS : zeroS
    ),
  );

let rec pow = (a: value, b: value): value =>
  switch (a, b) {
  | (`Zero, `Real(_) | `Imag(_) | `Complex(_)) => `Zero
  | (`Real(_) | `Imag(_) | `Complex(_), `Zero) => one
  | (`Zero, `Zero) => `NaN
  | (`Real(re), `Real(Rational(1, 2, Unit))) =>
    let q = Q.abs(aReQ);
    let denSqrt = Q.den(q)->QUtil.sqrtZ;
    let (q, c) =
      if (QUtil.isInt(denSqrt) && Constant.(aReC == Unit)) {
        (Q.inv(denSqrt), Constant.Sqrt(Q.num(q)));
      } else {
        (QCUtil.mapFloat(q, aReC, sqrt), Unit);
      };
    if (Q.(aReQ > zero)) {
      realQC(q, c);
    } else {
      imagQC(q, c);
    };
  | (`Real(Rational(1, 1, Exp(1))), _) => exp(b)
  | (_, `Real(Rational(2, 1, Unit))) => a * a
  | (`Real(Rational(_, _, Unit)), `Real(Rational(i, 1, Unit))) =>
    switch (Q.num(isInt)->Z.to_int) {
    | bn => real(QUtil.pow(aReQ, bn))
    | exception Z.Overflow => `NaN
    }
  | (`Real(Rational(_, _, Unit)), `Real(Rational(ltZero, 0, Unit)))
      when ltZero < 0 =>
    pow(real(Q.inv(aReQ)), real(Q.abs(ltZero)))
  | (`Imag(im), `Real(Rational(_, 1, Unit))) =>
    let aPowB = pow(realQC(im), b);
    switch (Q.num(isInt)->ZUtil.safeMod(Z.of_int(4))->Z.to_int) {
    | 0 => aPowB
    | 1 => aPowB * i
    | 2 => - aPowB
    | 3 => aPowB * minusI
    | _ => raise(Not_found)
    };
  | (`Real(_) | `Imag(_) | `Complex(_), `Real(_) | `Imag(_) | `Complex(_)) =>
    exp(log(a) * b)
  | (`Percent(aP), `Percent(bP)) =>
    pow(Base_Util.percentToNumerical(aP), Base_Util.percentToNumerical(bP))
  | (`Percent(p), _) => pow(Base_Util.percentToNumerical(p), b)
  | (_, `Percent(p)) => pow(a, Base_Util.percentToNumerical(p))
  | (
      `Matrix({numRows: 2, numColumns: 2, elements: [|a, b, c, d|]}),
      `Real(Rational((-1), 1, Unit)),
    )
      when Q.(isMinusOne == minus_one) =>
    let (~-) = Base_Functions.negScalar;
    let (-) = Base_Operators.subScalar;
    let ( * ) = Base_Operators.mulScalar;
    let (/) = Base_Operators.divScalar;
    let factor = a * d - b * c;
    matrix2(d / factor, - b / factor, - c / factor, a / factor);
  | (
      `Matrix({
        numRows: 3,
        numColumns: 3,
        elements: [|a, b, c, d, e, f, g, h, i|],
      }),
      `Real(Rational((-1), 1, Unit)),
    )
      when Q.(isMinusOne == minus_one) =>
    /* https://www.wolframalpha.com/input/?i=%7B%7Ba,b,c%7D,%7Bd,e,f%7D,%7Bg,h,i%7D%7D%5E-1 */
    let (+) = Base_Operators.addScalar;
    let (-) = Base_Operators.subScalar;
    let ( * ) = Base_Operators.mulScalar;
    let (/) = Base_Operators.divScalar;
    let factor =
      a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
    matrix3(
      (e * i - f * h) / factor,
      (c * h - b * i) / factor,
      (b * f - c * e) / factor,
      (f * g - d * i) / factor,
      (a * i - c * g) / factor,
      (c * d - a * f) / factor,
      (d * h - e * g) / factor,
      (b * g - a * h) / factor,
      (a * e - b * d) / factor,
    );
  | (
      `Matrix(
        ({numRows: 2, numColumns: 2} | {numRows: 3, numColumns: 3}) as aM,
      ),
      `Zero,
    ) =>
    toIdentity(aM)
  | (`Matrix(aM) as a, `Real(Rational(gtZero, 1, Unit))) when gtZero >= 0 =>
    let x = ref(toIdentity(aM));
    for (_ in 0 to Pervasives.(Q.num(intGtZero)->Z.to_int - 1)) {
      x := x^ * a;
    };
    x^;
  | (`NaN | `Vector(_) | `Matrix(_), _) => `NaN
  | (_, `NaN | `Vector(_) | `Matrix(_)) => `NaN
  };

let sqrt = (x: value): value => pow(x, half);