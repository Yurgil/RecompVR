#ifndef RECOMPVR_ANDROID_H
#define RECOMPVR_ANDROID_H

#include <jni.h>
#include <openxr/openxr.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize before querying runtime extensions or creating an instance.
 * Supply the activity from the application's JNI or NativeActivity owner. */
XrResult recompvr_initialize_android_loader(JavaVM *vm, jobject activity);

#ifdef __cplusplus
}
#endif
#endif
