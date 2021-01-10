#pragma once

#include "gesture.h"

#include "grabber.h"
#include "actiondb.h"
#include "events/windowobserver.h"

class Handler;

class EventLoop {
    friend class Handler;
public:
    EventLoop();

    bool handle(Glib::IOCondition);
    void handle_event(XEvent &ev);
    void handle_xi2_event(XIDeviceEvent *event);
    void handle_raw_motion(XIRawEvent *event);
    void report_xi2_event(XIDeviceEvent *event, const char *type);

    void fake_core_button(guint b, bool press);
    void fake_click(guint b);
    void update_core_mapping();

    void remove_device(int deviceid);
    void ungrab(int deviceid);

    bool idle();

    void queue(sigc::slot<void> f);

    Grabber::XiDevice *current_dev;
    bool in_proximity;
    std::set<guint> xinput_pressed;
    guint modifiers;
    std::map<guint, guint> core_inv_map;
private:
    Events::WindowObserver windowObserver;
    std::unique_ptr<Handler> handler;

    static int xErrorHandler(Display *dpy2, XErrorEvent *e);
    static int xIOErrorHandler(Display *dpy2);
    int (*oldHandler)(Display *, XErrorEvent *);
    int (*oldIOHandler)(Display *);
    std::list<sigc::slot<void> > queued;
    std::map<int, std::string> opcodes;
};

extern EventLoop *global_eventLoop;