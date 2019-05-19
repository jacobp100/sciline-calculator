open Types;
open OutputFormat;

let maxNaturalDenom = Z.of_int(1_000_000);
let maxNaturalQ = Q.of_int(100_000_000);

let formatNumber = (x, format) =>
  switch (format.mode) {
  | String
  | Tex => x
  | MathML => "<mn>" ++ x ++ "</mn>"
  };

let formatExponential = ((base, exponent), format) =>
  switch (format.mode) {
  | String => base ++ "e" ++ exponent
  | Tex => base ++ "*10^{" ++ exponent ++ "}"
  | MathML =>
    formatNumber(base, format)
    ++ "<mo>&times;</mo><msup><mn>10</mn>"
    ++ formatNumber(exponent, format)
    ++ "</msup>"
  };

let formatOperator = (op, format) =>
  format.mode == MathML ? "<mo>" ++ op ++ "</mo>" : op;

let formatVariable = (var, format) =>
  format.mode == MathML ? "<mi>" ++ var ++ "</mi>" : var;

let formatTuple = (q: Q.t, c: Constant.t, format): string => {
  let base = format.base;

  switch (format.style) {
  | Natural when Z.(Q.den(q) < maxNaturalDenom) && Q.(abs(q) < maxNaturalQ) =>
    let (num, den) = (Q.num(q), Q.den(q));
    let formatting = NumberFormat.createFormat(~digitSeparators=true, ());
    let minus = Q.(q < zero) ? formatOperator("-", format) : "";

    let (top, needsWrap) =
      switch (
        NumberFormat.formatInteger(~base, formatting, Z.abs(num)),
        Constant.toString(~format, c),
      ) {
      | ("1", "") => (formatNumber("1", format), false)
      | ("1", constant) => (constant, false)
      | (numerator, constant) => (
          formatNumber(numerator, format) ++ constant,
          true,
        )
      };

    switch (format.mode, NumberFormat.formatInteger(~base, formatting, den)) {
    | (_, "1") => minus ++ top
    | (String, bottom) => minus ++ top ++ "/" ++ bottom
    | (Tex, bottom) => minus ++ "\\frac{" ++ top ++ "}{" ++ bottom ++ "}"
    | (MathML, denominator) =>
      let top = needsWrap ? "<mrow>" ++ top ++ "</mrow>" : top;
      let bottom = formatNumber(denominator, format);
      minus ++ "<mfrac>" ++ top ++ bottom ++ "</mfrac>";
    };
  | Natural
  | Decimal =>
    let q = QCUtil.toQ(q, c);
    let floatVal = Q.to_float(q);
    let valueMagnitude = floor(log10(abs_float(floatVal)));
    let insideMagnitudeThreshold =
      valueMagnitude >= format.decimalMinMagnitude
      && valueMagnitude <= format.decimalMaxMagnitude;

    if (insideMagnitudeThreshold) {
      NumberFormat.formatDecimal(
        ~base,
        NumberFormat.createFormat(
          ~maxDecimalPlaces=format.precision,
          ~digitSeparators=valueMagnitude >= 5.,
          (),
        ),
        q,
      )
      ->formatNumber(format);
    } else {
      NumberFormat.formatExponential(
        ~base,
        ~exponent=QUtil.magnitude(q),
        NumberFormat.createFormat(~maxDecimalPlaces=format.precision, ()),
        q,
      )
      ->formatExponential(format);
    };
  | Scientific =>
    /* Round to multiple of 3 */
    let q = QCUtil.toQ(q, c);
    let exponent = QUtil.magnitude(q) * 3 / 3;
    let formatting =
      NumberFormat.createFormat(
        ~minDecimalPlaces=format.precision,
        ~maxDecimalPlaces=format.precision,
        (),
      );
    NumberFormat.formatExponential(~base, ~exponent, formatting, q)
    ->formatExponential(format);
  };
};

let formatImagTuple = (q: Q.t, c: Constant.t, format): string => {
  let i = formatVariable("i", format);
  switch (format.style, q, c) {
  | (Natural | Decimal, isOne, Unit) when Q.(isOne == one) => i
  | (Natural | Decimal, isMinusOne, Unit) when Q.(isMinusOne == minus_one) =>
    formatOperator("-", format) ++ i
  | (_, q, c) => formatTuple(q, c, format) ++ i
  };
};

let formatScalar = (a: scalar, format): string =>
  switch (a) {
  | `Zero => formatNumber("0", format)
  | `Real(q, c) => formatTuple(q, c, format)
  | `Imag(q, c) => formatImagTuple(q, c, format)
  | `Complex(reQ, reC, imQ, imC) =>
    formatTuple(reQ, reC, format)
    ++ formatOperator(Q.(imQ < zero) ? "-" : "+", format)
    ++ formatImagTuple(Q.abs(imQ), imC, format)
  };

let toString = (~format=default, ~inline=false, a: value): string => {
  let body =
    switch (a) {
    | (`Zero | `Real(_) | `Imag(_) | `Complex(_)) as aV =>
      formatScalar(aV, format)
    | (`Matrix(_) | `Vector(_)) as aV =>
      let m =
        switch (aV) {
        | `Matrix(m) => m
        | `Vector(elements) => Matrix.ofVector(elements)
        };
      open MatrixFormat;
      let fmt =
        switch (format.mode) {
        | String => matrixFormatString
        | Tex => matrixFormatTex
        | MathML => matrixFormatMathML
        };
      formatMatrix(Matrix.map(m, s => formatScalar(s, format)), fmt);
    | `NaN => formatVariable("NaN", format)
    };

  switch (format.mode) {
  | String
  | Tex => body
  | MathML =>
    let display = inline ? "inline" : "block";
    "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\""
    ++ display
    ++ "\">"
    ++ body
    ++ "</math>";
  };
};
