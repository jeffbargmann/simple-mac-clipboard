const tryRelease = () => {
  try {
    // prevent webpack error
    const type = 'Release'
    return require(`./build/${type}/simple_mac_clipboard.node`)
  } catch (e) {
    //  noop
  }
}
const tryDebug = () => {
  try {
    // prevent webpack error
    const type = 'Debug'
    return require(`./build/${type}/simple_mac_clipboard.node`)
  } catch (e) {
    //  noop
  }
}
const bin = tryRelease() || tryDebug() || require('bindings')('simple_mac_clipboard')
const {path, clearContents, getChangeCount, hasBufferForFormat, dataForType, setData} = bin

// alias
const clear = clearContents
const readBuffer = dataForType
const writeBuffer = setData

// convenient
// format is the first parameter & required
const readText = format => readBuffer(format)?.toString('utf8')
const writeText = (format, text) => writeBuffer(format, Buffer.from(text))

// format
const FORMAT_PLAIN_TEXT = 'public.utf8-plain-text'
const FORMAT_FILE_URL = 'public.file-url'

Object.assign(module.exports, {
  path, // the .node path

  clear,
  getChangeCount,
  hasBufferForFormat,
  readBuffer,
  writeBuffer,
  readText,
  writeText,

  FORMAT_PLAIN_TEXT,
  FORMAT_FILE_URL,
})
