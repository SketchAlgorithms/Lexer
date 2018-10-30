const Wasm = require(".");

const fromPointer = ptr => {
  // Retransform back your pointer to string using 'Glue' method
  let resValue = Wasm.Pointer_stringify(ptr);
  // Free the memory allocated by 'allocate'
  Wasm._free(ptr);
  return resValue;
};

const strToPtr = str => {
  return Wasm.allocateUTF8(str);
};

const setInput = str => {
  const data = strToPtr(str);
  Wasm._setInput(data);
};

const getNextToken = () => {
  const json = fromPointer(Wasm._getNextToken());
  return JSON.parse(json);
};

module.exports = {
  setInput,
  getNextToken
};
