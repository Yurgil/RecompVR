#ifndef RECOMPVR_OPENXR_H
#define RECOMPVR_OPENXR_H

#include <stdbool.h>
#include <openxr/openxr.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { RECOMPVR_EYE_COUNT = 2 };

XrResult recompvr_begin_stereo_session(XrSession session);
XrResult recompvr_create_reference_space(XrSession session, XrReferenceSpaceType type, XrSpace *space);
XrResult recompvr_wait_frame(XrSession session, XrFrameState *state);
XrResult recompvr_begin_frame(XrSession session);

/* Views and state are initialized by this call. Check the result before reading either.
 * A valid estimated pose is sufficient here. Callers may require tracked bits separately. */
XrResult recompvr_locate_stereo(XrSession session, XrSpace space, XrTime time,
                               XrView eyes[RECOMPVR_EYE_COUNT], XrViewState *state, uint32_t *count);
bool recompvr_stereo_valid(const XrViewState *state, uint32_t count);

bool recompvr_read_button(XrSession session, const XrActionStateGetInfo *info);
float recompvr_read_float(XrSession session, const XrActionStateGetInfo *info);
XrVector2f recompvr_read_stick(XrSession session, const XrActionStateGetInfo *info);
bool recompvr_locate_action(XrSession session, const XrActionStateGetInfo *info, XrSpace action_space,
                            XrSpace base_space, XrTime time, XrSpaceLocationFlags required, XrPosef *pose);

#ifdef __cplusplus
}
#endif
#endif
