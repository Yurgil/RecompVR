#include "recompvr/openxr.h"

#include <assert.h>

static XrBool32 active;
static XrResult status;
static XrViewStateFlags view_flags;
static uint32_t view_count;
static XrSpaceLocationFlags location_flags;
static unsigned location_calls;

XRAPI_ATTR XrResult XRAPI_CALL xrBeginSession(XrSession session, const XrSessionBeginInfo *info)
{
    assert(session == XR_NULL_HANDLE && info->type == XR_TYPE_SESSION_BEGIN_INFO && !info->next);
    assert(info->primaryViewConfigurationType == XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO);
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrCreateReferenceSpace(XrSession session,
    const XrReferenceSpaceCreateInfo *info, XrSpace *space)
{
    assert(session == XR_NULL_HANDLE && info->type == XR_TYPE_REFERENCE_SPACE_CREATE_INFO && !info->next);
    assert(info->referenceSpaceType == XR_REFERENCE_SPACE_TYPE_VIEW);
    assert(info->poseInReferenceSpace.orientation.w == 1.0f && info->poseInReferenceSpace.position.z == 0.0f);
    *space = XR_NULL_HANDLE;
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrWaitFrame(XrSession session, const XrFrameWaitInfo *info, XrFrameState *state)
{
    assert(session == XR_NULL_HANDLE && info->type == XR_TYPE_FRAME_WAIT_INFO);
    assert(state->type == XR_TYPE_FRAME_STATE && !state->next);
    state->predictedDisplayTime = 42;
    state->shouldRender = XR_FALSE;
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrBeginFrame(XrSession session, const XrFrameBeginInfo *info)
{
    assert(session == XR_NULL_HANDLE && info->type == XR_TYPE_FRAME_BEGIN_INFO && !info->next);
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrLocateViews(XrSession session, const XrViewLocateInfo *info, XrViewState *state,
    uint32_t capacity, uint32_t *count, XrView *eyes)
{
    assert(session == XR_NULL_HANDLE && info->type == XR_TYPE_VIEW_LOCATE_INFO);
    assert(info->displayTime == 42 && info->viewConfigurationType == XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO);
    assert(state->type == XR_TYPE_VIEW_STATE && !state->next && capacity == 2 && *count == 0);
    for (unsigned eye = 0; eye < capacity; ++eye)
        assert(eyes[eye].type == XR_TYPE_VIEW && !eyes[eye].next);
    state->viewStateFlags = view_flags;
    *count = view_count;
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrGetActionStateBoolean(XrSession session,
    const XrActionStateGetInfo *info, XrActionStateBoolean *state)
{
    assert(session == XR_NULL_HANDLE && info->subactionPath == 3 && state->type == XR_TYPE_ACTION_STATE_BOOLEAN);
    state->isActive = active;
    state->currentState = XR_TRUE;
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrGetActionStateFloat(XrSession session,
    const XrActionStateGetInfo *info, XrActionStateFloat *state)
{
    assert(session == XR_NULL_HANDLE && info->subactionPath == 3 && state->type == XR_TYPE_ACTION_STATE_FLOAT);
    state->isActive = active;
    state->currentState = 0.75f;
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrGetActionStateVector2f(XrSession session,
    const XrActionStateGetInfo *info, XrActionStateVector2f *state)
{
    assert(session == XR_NULL_HANDLE && info->subactionPath == 3 && state->type == XR_TYPE_ACTION_STATE_VECTOR2F);
    state->isActive = active;
    state->currentState = (XrVector2f){ -0.25f, 0.75f };
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrGetActionStatePose(XrSession session,
    const XrActionStateGetInfo *info, XrActionStatePose *state)
{
    assert(session == XR_NULL_HANDLE && info->subactionPath == 3 && state->type == XR_TYPE_ACTION_STATE_POSE);
    state->isActive = active;
    return status;
}

XRAPI_ATTR XrResult XRAPI_CALL xrLocateSpace(XrSpace space, XrSpace base, XrTime time, XrSpaceLocation *location)
{
    assert(space == XR_NULL_HANDLE && base == XR_NULL_HANDLE && time == 42);
    assert(location->type == XR_TYPE_SPACE_LOCATION);
    ++location_calls;
    location->locationFlags = location_flags;
    location->pose.position.x = 2.0f;
    return status;
}

int main(void)
{
    const XrSession session = XR_NULL_HANDLE;
    XrSpace space;
    assert(recompvr_create_reference_space(session, XR_REFERENCE_SPACE_TYPE_VIEW, &space) == XR_SUCCESS);
    assert(recompvr_begin_stereo_session(session) == XR_SUCCESS);
    XrFrameState frame;
    assert(recompvr_wait_frame(session, &frame) == XR_SUCCESS && !frame.shouldRender);
    status = XR_FRAME_DISCARDED;
    assert(recompvr_begin_frame(session) == XR_FRAME_DISCARDED);

    XrView eyes[RECOMPVR_EYE_COUNT];
    XrViewState views;
    uint32_t count;
    status = XR_ERROR_SESSION_LOST;
    assert(recompvr_locate_stereo(session, space, frame.predictedDisplayTime, eyes, &views, &count) == status);
    status = XR_SUCCESS;
    view_count = 1;
    view_flags = XR_VIEW_STATE_POSITION_VALID_BIT | XR_VIEW_STATE_ORIENTATION_VALID_BIT;
    assert(recompvr_locate_stereo(session, space, frame.predictedDisplayTime, eyes, &views, &count) == XR_SUCCESS);
    assert(!recompvr_stereo_valid(&views, count));
    view_count = 2;
    assert(recompvr_locate_stereo(session, space, frame.predictedDisplayTime, eyes, &views, &count) == XR_SUCCESS);
    assert(recompvr_stereo_valid(&views, count));
    views.viewStateFlags &= ~XR_VIEW_STATE_POSITION_VALID_BIT;
    assert(!recompvr_stereo_valid(&views, count));

    const XrActionStateGetInfo input = { .type = XR_TYPE_ACTION_STATE_GET_INFO, .subactionPath = 3 };
    assert(!recompvr_read_button(session, &input) && recompvr_read_float(session, &input) == 0.0f);
    assert(recompvr_read_stick(session, &input).x == 0.0f);
    XrPosef pose = {0};
    const XrSpaceLocationFlags valid = XR_SPACE_LOCATION_POSITION_VALID_BIT | XR_SPACE_LOCATION_ORIENTATION_VALID_BIT;
    assert(!recompvr_locate_action(session, &input, space, space, frame.predictedDisplayTime, valid, &pose));
    assert(location_calls == 0 && pose.position.x == 0.0f);
    active = XR_TRUE;
    location_flags = valid;
    assert(recompvr_read_button(session, &input) && recompvr_read_float(session, &input) == 0.75f);
    assert(recompvr_read_stick(session, &input).x == -0.25f);
    const XrSpaceLocationFlags tracked = valid | XR_SPACE_LOCATION_POSITION_TRACKED_BIT |
                                        XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT;
    assert(!recompvr_locate_action(session, &input, space, space, frame.predictedDisplayTime, tracked, &pose));
    assert(pose.position.x == 0.0f);
    assert(recompvr_locate_action(session, &input, space, space, frame.predictedDisplayTime, valid, &pose));
    assert(pose.position.x == 2.0f);
    status = XR_ERROR_SESSION_LOST;
    assert(!recompvr_read_button(session, &input) && recompvr_read_float(session, &input) == 0.0f);
    assert(recompvr_read_stick(session, &input).y == 0.0f);
}
