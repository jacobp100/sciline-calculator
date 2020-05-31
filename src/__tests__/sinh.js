const mathjs = require("mathjs");
const { toMatchJsValue } = require("../__test-util__");
const { imagValues } = require("../__test-util__/math-js");
const TechniCalc = require("../Value.bs");

expect.extend({ toMatchJsValue });

test.each(imagValues)("sinh(%s)", (v) => {
  const actual = TechniCalc.sinh(v.techniCalcValue);
  const expected = mathjs.sinh(v.jsValue);
  expect(actual).toMatchJsValue(expected);
});
