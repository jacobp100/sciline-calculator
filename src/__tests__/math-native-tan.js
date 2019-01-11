const { toMatchJsValue } = require("../testUtil");
const { trigValues } = require("../testUtil/math-native");
const SciLine = require("../SciLine.bs");

expect.extend({ toMatchJsValue });

const tanInfiniteValues = new Set([
  "1pi/2",
  "3pi/2",
  "5pi/2",
  "7pi/2",
  "9pi/2",
  "11pi/2",
  "-1pi/2",
  "-3pi/2",
  "-5pi/2",
  "-7pi/2",
  "-9pi/2",
  "-11pi/2"
]);

trigValues.forEach(v => {
  it(v.title, () => {
    const sciLineValue = SciLine.tan(v.sciLineValue);
    if (tanInfiniteValues.has(v.title)) {
      expect(SciLine.is_nan).toBeTruthy();
    } else {
      expect(sciLineValue).toMatchJsValue(Math.tan(v.jsValue));
    }
  });
});
