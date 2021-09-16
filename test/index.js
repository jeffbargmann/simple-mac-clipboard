const should = require('should')
const clip = require('..')
const {clipboard} = require('electron')

describe('write', () => {
  it.skip('demo ', () => {
    // demo code
  })

  it('.clear ', () => {
    clip.clear()
    clipboard.readText().length.should.equal(0)
  })

  it('.getChangeCount ', () => {
    clipboard.readText().length.should.not.equal(0)
  })
})
