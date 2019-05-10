const mathjs = require("mathjs");
const { toMatchJsValue } = require("../__test-util__");
const { trigValues } = require("../__test-util__/math-js");
const SciLine = require("../Value.bs");

expect.extend({ toMatchJsValue });

const cosRealOnlyValues = new Set([
  "(1/1+7i/1)pi",
  "(1/1+8i/1)pi",
  "(1/1+9i/1)pi",
  "(1/1+10i/1)pi",
  "(1/1+11i/1)pi",
  "(1/1+12i/1)pi",
  "(2/1+7i/1)pi",
  "(2/1+8i/1)pi",
  "(2/1+9i/1)pi",
  "(2/1+10i/1)pi",
  "(2/1+11i/1)pi",
  "(2/1+12i/1)pi",
  "(3/1+7i/1)pi",
  "(3/1+8i/1)pi",
  "(3/1+9i/1)pi",
  "(3/1+10i/1)pi",
  "(3/1+11i/1)pi",
  "(3/1+12i/1)pi",
  "(4/1+7i/1)pi",
  "(4/1+8i/1)pi",
  "(4/1+9i/1)pi",
  "(4/1+10i/1)pi",
  "(4/1+11i/1)pi",
  "(4/1+12i/1)pi",
  "(5/1+7i/1)pi",
  "(5/1+8i/1)pi",
  "(5/1+9i/1)pi",
  "(5/1+10i/1)pi",
  "(5/1+11i/1)pi",
  "(5/1+12i/1)pi",
  "(6/1+7i/1)pi",
  "(6/1+8i/1)pi",
  "(6/1+9i/1)pi",
  "(6/1+10i/1)pi",
  "(6/1+11i/1)pi",
  "(6/1+12i/1)pi",
  "(7/1+7i/1)pi",
  "(7/1+8i/1)pi",
  "(7/1+9i/1)pi",
  "(7/1+10i/1)pi",
  "(7/1+11i/1)pi",
  "(7/1+12i/1)pi",
  "(8/1+7i/1)pi",
  "(8/1+8i/1)pi",
  "(8/1+9i/1)pi",
  "(8/1+10i/1)pi",
  "(8/1+11i/1)pi",
  "(8/1+12i/1)pi",
  "(9/1+7i/1)pi",
  "(9/1+8i/1)pi",
  "(9/1+9i/1)pi",
  "(9/1+10i/1)pi",
  "(9/1+11i/1)pi",
  "(9/1+12i/1)pi",
  "(10/1+7i/1)pi",
  "(10/1+8i/1)pi",
  "(10/1+9i/1)pi",
  "(10/1+10i/1)pi",
  "(10/1+11i/1)pi",
  "(10/1+12i/1)pi",
  "(11/1+6i/1)pi",
  "(11/1+7i/1)pi",
  "(11/1+8i/1)pi",
  "(11/1+9i/1)pi",
  "(11/1+10i/1)pi",
  "(11/1+11i/1)pi",
  "(11/1+12i/1)pi",
  "(12/1+6i/1)pi",
  "(12/1+7i/1)pi",
  "(12/1+8i/1)pi",
  "(12/1+9i/1)pi",
  "(12/1+10i/1)pi",
  "(12/1+11i/1)pi",
  "(12/1+12i/1)pi"
]);

const cosImaginaryOnlyValues = new Set([
  "(1/2+7i/1)pi",
  "(1/2+8i/1)pi",
  "(1/2+9i/1)pi",
  "(1/2+10i/1)pi",
  "(1/2+11i/1)pi",
  "(1/2+12i/1)pi",
  "(3/2+7i/1)pi",
  "(3/2+8i/1)pi",
  "(3/2+9i/1)pi",
  "(3/2+10i/1)pi",
  "(3/2+11i/1)pi",
  "(3/2+12i/1)pi",
  "(5/2+7i/1)pi",
  "(5/2+8i/1)pi",
  "(5/2+9i/1)pi",
  "(5/2+10i/1)pi",
  "(5/2+11i/1)pi",
  "(5/2+12i/1)pi",
  "(7/2+7i/1)pi",
  "(7/2+8i/1)pi",
  "(7/2+9i/1)pi",
  "(7/2+10i/1)pi",
  "(7/2+11i/1)pi",
  "(7/2+12i/1)pi",
  "(9/2+7i/1)pi",
  "(9/2+8i/1)pi",
  "(9/2+9i/1)pi",
  "(9/2+10i/1)pi",
  "(9/2+11i/1)pi",
  "(9/2+12i/1)pi",
  "(11/2+6i/1)pi",
  "(11/2+7i/1)pi",
  "(11/2+8i/1)pi",
  "(11/2+9i/1)pi",
  "(11/2+10i/1)pi",
  "(11/2+11i/1)pi",
  "(11/2+12i/1)pi"
]);

trigValues.forEach(v => {
  it(`cos ${v.title}`, () => {
    let mathJsValue = mathjs.cos(v.jsValue);
    if (cosRealOnlyValues.has(v.title)) {
      mathJsValue = { re: mathJsValue.re, im: 0 };
    } else if (cosImaginaryOnlyValues.has(v.title)) {
      mathJsValue = { re: 0, im: mathJsValue.im };
    }
    expect(SciLine.cos(v.sciLineValue)).toMatchJsValue(mathJsValue);
  });
});
