#include "recompvr/openxr.h"

XrResult recompvr_begin_stereo_session(XrSession session)
{
    const XrSessionBeginInfo info = { .type = XR_TYPE_SESSION_BEGIN_INFO,
        .primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO };
    return xrBeginSession(session, &info);
}

XrResult recompvr_create_reference_space(XrSession session, XrReferenceSpaceType type, XrSpace *space)
{
    const XrReferenceSpaceCreateInfo info = { .type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
        .referenceSpaceType = type, .poseInReferenceSpace.orientation.w = 1.0f };
    return xrCreateReferenceSpace(session, &info, space);
}

XrResult recompvr_wait_frame(XrSession session, XrFrameState *state)
{
    const XrFrameWaitInfo info = { .type = XR_TYPE_FRAME_WAIT_INFO };
    *state = (XrFrameState){ .type = XR_TYPE_FRAME_STATE };
    return xrWaitFrame(session, &info, state);
}

XrResult recompvr_begin_frame(XrSession session)
{
    const XrFrameBeginInfo info = { .type = XR_TYPE_FRAME_BEGIN_INFO };
    return xrBeginFrame(session, &info);
}

XrResult recompvr_locate_stereo(XrSession session, XrSpace space, XrTime time,
                               XrView eyes[RECOMPVR_EYE_COUNT], XrViewState *state, uint32_t *count)
{
    const XrViewLocateInfo info = { .type = XR_TYPE_VIEW_LOCATE_INFO,
        .viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
        .displayTime = time, .space = space };
    *state = (XrViewState){ .type = XR_TYPE_VIEW_STATE };
    *count = 0;
    for (unsigned eye = 0; eye < RECOMPVR_EYE_COUNT; ++eye)
        eyes[eye] = (XrView){ .type = XR_TYPE_VIEW };
    return xrLocateViews(session, &info, state, RECOMPVR_EYE_COUNT, count, eyes);
}

bool recompvr_stereo_valid(const XrViewState *state, uint32_t count)
{
    const XrViewStateFlags required = XR_VIEW_STATE_POSITION_VALID_BIT | XR_VIEW_STATE_ORIENTATION_VALID_BIT;
    return count == RECOMPVR_EYE_COUNT && (state->viewStateFlags & required) == required;
}

bool recompvr_read_button(XrSession session, const XrActionStateGetInfo *info)
{
    XrActionStateBoolean state = { .type = XR_TYPE_ACTION_STATE_BOOLEAN };
    return XR_SUCCEEDED(xrGetActionStateBoolean(session, info, &state)) &&
        state.isActive && state.currentState;
}

float recompvr_read_float(XrSession session, const XrActionStateGetInfo *info)
{
    XrActionStateFloat state = { .type = XR_TYPE_ACTION_STATE_FLOAT };
    return XR_SUCCEEDED(xrGetActionStateFloat(session, info, &state)) && state.isActive
        ? state.currentState : 0.0f;
}

XrVector2f recompvr_read_stick(XrSession session, const XrActionStateGetInfo *info)
{
    XrActionStateVector2f state = { .type = XR_TYPE_ACTION_STATE_VECTOR2F };
    return XR_SUCCEEDED(xrGetActionStateVector2f(session, info, &state)) && state.isActive
        ? state.currentState : (XrVector2f){0};
}

bool recompvr_locate_action(XrSession session, const XrActionStateGetInfo *info, XrSpace action_space,
                            XrSpace base_space, XrTime time, XrSpaceLocationFlags required, XrPosef *pose)
{
    XrActionStatePose state = { .type = XR_TYPE_ACTION_STATE_POSE };
    XrSpaceLocation location = { .type = XR_TYPE_SPACE_LOCATION };
    if (XR_FAILED(xrGetActionStatePose(session, info, &state)) || !state.isActive ||
        XR_FAILED(xrLocateSpace(action_space, base_space, time, &location)) ||
        (location.locationFlags & required) != required)
        return false;
    *pose = location.pose;
    return true;
}
