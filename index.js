const { platform } = require('os');

module.exports = {
  getWindowText: () => {}
};

if (platform() === 'win32') {
  module.exports = require('bindings')('NativeExtension');
}
