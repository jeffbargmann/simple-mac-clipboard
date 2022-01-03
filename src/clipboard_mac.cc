#include <iostream>
#include <napi.h>

#ifdef __APPLE__
#import <AppKit/AppKit.h>
#import <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import <objc/objc-runtime.h>
#endif

using namespace std;
using namespace Napi;

#ifdef __APPLE__
NSString *getStringArg(const CallbackInfo &info, int index) {
  Napi::String s = info[index].ToString();
  std::string valInStdString(s);
  NSString *val = [NSString stringWithUTF8String:valInStdString.c_str()];
  return val;
}

Value clearContents(const CallbackInfo &info) {
#ifdef DEBUG
  NSLog(@"clearContents ...");
#endif

  // clear
  [NSPasteboard.generalPasteboard clearContents];

  return info.Env().Undefined();
}

Value getChangeCount(const CallbackInfo &info) {
#ifdef DEBUG
  NSLog(@"getChangeCount ...");
#endif

  // get
  return Napi::Number::New(info.Env(), [NSPasteboard.generalPasteboard changeCount]);
}

Value setData(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() < 2) {
    Napi::Error::New(env, "wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }
  if (!info[0].IsString() || !info[1].IsObject()) {
    Napi::Error::New(
        env, "arguments type does not match (format: string, buf: Buffer)")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  // arg1: format
  NSString *format = getStringArg(info, 0);

  // arg2: buf
  // char *buf = node::Buffer::Data(info[1]);
  // size_t length = node::Buffer::Length(info[1]);
  Buffer<uint8_t> b = info[1].As<Buffer<uint8_t>>();
  uint8_t *buf = b.Data();
  size_t length = b.Length();

  // no copy
  // this will cause electron app to freeze, don't know why
  // NSData *data = [NSData dataWithBytesNoCopy:buf
  //                                     length:(NSUInteger)length
  //                               freeWhenDone:NO];

  // copy
  NSData *data = [NSData dataWithBytes:buf length:(NSUInteger)length];
#ifdef DEBUG
  NSLog(@"setData: format=%@, buf.length=%zu", format, length);
#endif

  BOOL isText = [format isEqualToString:@"public.utf8-plain-text"]];
  if(isText)
  {
    [NSPasteboard.generalPasteboard declareTypes:@[ format ] owner:nil];
    BOOL success = [NSPasteboard.generalPasteboard setData:data forType:format];
#ifdef DEBUG
    NSLog(@"writeTextToClipboard: result = %i", success);
#endif
  }
  else
  {
    //NSDictionary *dict = (__bridge NSString *)UTTypeCopyDeclaration(CFSTR(format));

    NSString *tempFilePath = [NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    NSURL *tempFileUrl = [NSURL fileURLWithPath:tempFilePath];

    NSError *error;
    BOOL success = [data writeToURL:tempFileUrl options:NSDataWritingAtomic error:&error];

    [NSPasteboard.generalPasteboard declareTypes:@[ format ] owner:nil];
    [NSPasteboard.generalPasteboard writeObjects:@[ tempFileUrl ]];
#ifdef DEBUG
    NSLog(@"writeDataToClipboard: result = %i", success);
#endif
  }

  [pool drain];
  return Napi::Boolean::New(env, success);
}

Value dataForType(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() < 1) {
    Napi::Error::New(env, "wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }
  if (!info[0].IsString()) {
    Napi::Error::New(env, "string expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  NSString *dataType = getStringArg(info, 0);
  NSData *data = [NSPasteboard.generalPasteboard dataForType:dataType];
  if(!data) {
    [pool drain];
    return env.Null();
  }
  uint8_t *pData = (uint8_t *)data.bytes;
  uint32_t len = (uint32_t)data.length;
  Napi::Buffer<uint8_t> buf = Napi::Buffer<uint8_t>::Copy(env, pData, len);

  [pool drain];
  return buf;
}

Value hasBufferForFormat(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() < 1) {
    Napi::Error::New(env, "wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }
  if (!info[0].IsString()) {
    Napi::Error::New(env, "string expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  NSString *dataType = getStringArg(info, 0);
  NSArray<NSString *> *dataTypeArray = @[dataType];
  BOOL success = [NSPasteboard.generalPasteboard canReadItemWithDataConformingToTypes:dataTypeArray];

  [pool drain];
  return Napi::Boolean::New(env, success);
}
#endif

Object Init(Env env, Object exports) {
  HandleScope scope(env);
#ifdef __APPLE__
  // clear
  exports.Set(String::New(env, "clearContents"),
              Function::New(env, clearContents));

  // get
  exports.Set(String::New(env, "getChangeCount"),
              Function::New(env, getChangeCount));

  // write
  exports.Set(String::New(env, "setData"), Function::New(env, setData));

  // read
  exports.Set(String::New(env, "dataForType"), Function::New(env, dataForType));
  exports.Set(String::New(env, "hasBufferForFormat"), Function::New(env, hasBufferForFormat));
#endif

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)