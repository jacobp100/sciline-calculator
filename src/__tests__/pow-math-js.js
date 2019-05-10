const cartesian = require("cartesian");
const { range } = require("lodash");
const mathjs = require("mathjs");
const { Value, toMatchJsValue } = require("../__test-util__");
const SciLine = require("../Value.bs");

expect.extend({ toMatchJsValue });

const mathematicallyAccuratePow = (a, b) => {
  // MathJS seems to mess up when a == 0
  if (mathjs.equal(a, 0)) return mathjs.equal(b, 0) ? NaN : 0;
  if (mathjs.equal(b, 0)) return 1;
  return mathjs.pow(a, b);
};

const expValues = [].concat(
  cartesian([range(-2, 2 + 2), range(-2, 2 + 2)]).map(([re, im]) =>
    Value.complex(re, im)
  ),
  Value.e(1),
  Value.e(2),
  Value.pi(1),
  Value.pi(2)
);

cartesian([expValues, expValues]).forEach(([a, b]) => {
  it(`(${a.title}) ** (${b.title})`, () => {
    expect(SciLine.pow(a.sciLineValue, b.sciLineValue)).toMatchJsValue(
      mathematicallyAccuratePow(a.jsValue, b.jsValue)
    );
  });
});
