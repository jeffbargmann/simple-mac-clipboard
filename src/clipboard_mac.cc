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

  BOOL success = false;
  BOOL isText = [format isEqualToString:@"public.utf8-plain-text"];
  if(isText)
  {
    [NSPasteboard.generalPasteboard clearContents];
    [NSPasteboard.generalPasteboard declareTypes:@[ format ] owner:nil];
    success = [NSPasteboard.generalPasteboard setData:data forType:format];
//#ifdef DEBUG
    NSLog(@"writeTextToClipboard: result = %i", success);
//#endif
  }
  else if(true)
  {
    NSString *tempFileStringPath = @"/Users/jeff/iMessage/clipboard.gif";//[NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    NSString *tempFileStringUrl = @"file:///Users/jeff/iMessage/clipboard.gif";//[NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    NSURL *tempFileUrl = [NSURL URLWithString:tempFileStringUrl];
    NSURL *tempFileUrlFromPath = [NSURL fileURLWithPath:tempFileStringPath];

  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
      [pboard clearContents];
  NSPasteboardItem *pbitem = [[NSPasteboardItem alloc] init];
   [pbitem setData:data forType:(__bridge NSString*)kUTTypeGIF];
   [pbitem setData:[tempFileStringUrl dataUsingEncoding:NSUTF8StringEncoding] forType:@"public.file-url"];
    [pboard writeObjects:@[pbitem]];
  }
  else if(true)
  {
    //NSString *tempFilePath1 = @"/Users/jeff/clipboard.gif";//[NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    //NSString *tempFilePath = @"file:///Users/jeff/clipboard.gif";//[NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    //NSURL *tempFileUrl = [NSURL URLWithString:tempFilePath];

    //NSError *error;
    //BOOL successWrite = [data writeToURL:tempFileUrl atomically:true];
 
 
 
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
      [pboard clearContents];
[pboard declareTypes:@[NSURLPboardType] owner:nil];
[[NSURL URLWithString:@"file:///Users/jeff/clipboard.gif"] writeToPasteboard:pboard];
  }
  else if(true)
  {
    NSImage *image = [[NSImage alloc] initWithData:data];
    if (image != nil)
    {
      [NSPasteboard.generalPasteboard clearContents];
      NSArray *copiedObjects = [NSArray arrayWithObject:image];
      success = [NSPasteboard.generalPasteboard writeObjects:copiedObjects];
      NSLog(@"writeImageToClipboard: %i", success);
    }
    else
    {
      NSLog(@"writeImageToClipboard: noimage %i", success);
    }
  }
  else
  {
/*
NSDictionary *dict = (__bridge NSString *)UTTypeCopyDeclaration(CFSTR(format));
{
UTTypeConformsTo =     (
"public.data",
"public.composite-content"
);
UTTypeDescription = "Portable Document Format (PDF)";
UTTypeIconFiles =     (
"PDF~iphone.png",
"PDF@2x~iphone.png",
"PDFStandard~ipad.png",
"PDFStandard@2x~ipad.png",
"PDFScalable~ipad.png",
"PDFScalable@2x~ipad.png"
);
UTTypeIdentifier = "com.adobe.pdf";
UTTypeTagSpecification =     {
"com.apple.nspboard-type" = "Apple PDF pasteboard type";
"com.apple.ostype" = "PDF ";
"public.filename-extension" = pdf;
"public.mime-type" = "application/pdf";
};
}
*/
    NSString *tempFilePath1 = @"/Users/jeff/clipboard.gif";//[NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    NSString *tempFilePath = @"file:///Users/jeff/clipboard.gif";//[NSTemporaryDirectory() stringByAppendingPathComponent:@"clipboard.gif"];
    NSURL *tempFileUrl = [NSURL URLWithString:tempFilePath];

    NSError *error;
    BOOL successWrite = [data writeToURL:tempFileUrl atomically:true];
 
    [NSPasteboard.generalPasteboard clearContents];
    [NSPasteboard.generalPasteboard declareTypes:@[NSFilenamesPboardType] owner:nil];
    success = [NSPasteboard.generalPasteboard setPropertyList:@[ tempFilePath1 ] forType:NSFilenamesPboardType];
//#ifdef DEBUG
    NSLog(@"writeDataToClipboard: %@", tempFilePath1);
    NSLog(@"writeDataToClipboard: %@", tempFileUrl);
    NSLog(@"writeDataToClipboard: result1 = %i", successWrite);
    NSLog(@"writeDataToClipboard: result23 = %i", success);
        NSDictionary *utiDict = (__bridge NSDictionary *)UTTypeCopyDeclaration((__bridge CFStringRef)format);
    NSLog(@"writeDataToClipboard: result4 = %@", utiDict);

//#endif
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