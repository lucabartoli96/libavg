//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2014 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#include "WrapHelper.h"

#include "../anim/SimpleAnim.h"
#include "../anim/LinearAnim.h"
#include "../anim/EaseInOutAnim.h"
#include "../anim/ContinuousAnim.h"
#include "../anim/WaitAnim.h"
#include "../anim/ParallelAnim.h"
#include "../anim/StateAnim.h"

#include "../player/BoostPython.h"

using namespace boost::python;
using namespace avg;
namespace bp = boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(start_overloads, start, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(setState_overloads, StateAnim::setState, 1, 2);

AnimPtr fadeInDeprecated(const bp::object& node, long long duration,
        float max, const bp::object& stopCallback=bp::object())
{
    avgDeprecationWarning("1.9.0", "avg.fadeIn", "avg.Anim.fadeIn");
    return LinearAnim::fadeIn(node, duration, max, stopCallback);
}

AnimPtr fadeOutDeprecated(const bp::object& node, long long duration,
        const bp::object& stopCallback=bp::object())
{
    avgDeprecationWarning("1.9.0", "avg.fadeOut", "avg.Anim.fadeOut");
    return LinearAnim::fadeOut(node, duration, stopCallback);
}

void export_anim()
{
    from_python_sequence<std::vector<AnimPtr> >();
    from_python_sequence<std::vector<AnimState> >();
    
    class_<Anim, boost::noncopyable>("Anim", no_init)
        .def("setStartCallback", &Anim::setStartCallback)
        .def("setStopCallback", &Anim::setStopCallback)
        .def("abort", &Anim::abort)
        .def("isRunning", &Anim::isRunning)
        .def("getNumRunningAnims", AttrAnim::getNumRunningAnims)
        .staticmethod("getNumRunningAnims")
        .def("fadeIn", LinearAnim::fadeIn, (bp::arg("node"), bp::arg("duration"),
                bp::arg("max")=1.0, bp::arg("stopCallback")=object()))
        .staticmethod("fadeIn")
        .def("fadeOut", LinearAnim::fadeOut, (bp::arg("node"), bp::arg("duration"),
                bp::arg("stopCallback")=object()))
        .staticmethod("fadeOut")
        ;

    class_<AttrAnim, boost::shared_ptr<AttrAnim>, bases<Anim>, boost::noncopyable>
            ("AttrAnim", no_init)
        .def("start", &AttrAnim::start, start_overloads(args("bKeepAttr")))
        ;
 
    class_<SimpleAnim, boost::shared_ptr<SimpleAnim>, bases<AttrAnim>, 
            boost::noncopyable>("SimpleAnim", no_init)
        ;

    class_<LinearAnim, boost::shared_ptr<LinearAnim>, bases<SimpleAnim>, 
            boost::noncopyable>("LinearAnim", no_init)
        .def(init<const object&, const UTF8String&, long long, const object&,
                const object&, optional<bool, const object&, const object&> >
                ((bp::arg("node"), bp::arg("attrName"), bp::arg("duration"), 
                 bp::arg("startValue"), bp::arg("endValue"), bp::arg("useInt")=false,
                 bp::arg("startCallback")=object(), bp::arg("stopCallback")=object())))
        ;

    class_<EaseInOutAnim, boost::shared_ptr<EaseInOutAnim>, bases<SimpleAnim>, 
            boost::noncopyable>("EaseInOutAnim", no_init)
        .def(init<const object&, const UTF8String&, long long, const object&,
                const object&, long long, long long, 
                optional<bool, const object&, const object&> >
                ((bp::arg("node"), bp::arg("attrName"), bp::arg("duration"), 
                 bp::arg("startValue"), bp::arg("endValue"), bp::arg("easeInDuration"), 
                 bp::arg("easeOutDuration"), bp::arg("useInt")=false,
                 bp::arg("startCallback")=object(), bp::arg("stopCallback")=object())))
        ;
   
    class_<ContinuousAnim, boost::shared_ptr<ContinuousAnim>, bases<AttrAnim>, 
            boost::noncopyable>("ContinuousAnim", no_init)
        .def(init<const object&, const UTF8String&, const object&, const object&,
                optional<bool, const object&, const object&> >
                ((bp::arg("node"), bp::arg("attrName"), bp::arg("duration"), 
                 bp::arg("startValue"), bp::arg("speed"), bp::arg("useInt")=false,
                 bp::arg("startCallback")=object(), bp::arg("stopCallback")=object())))
        ;

    class_<WaitAnim, boost::shared_ptr<WaitAnim>, bases<Anim>, boost::noncopyable>(
            "WaitAnim", no_init)
        .def(init<optional<long long, const object&, const object&> >
                ((bp::arg("duration")=-1, bp::arg("startCallback")=object(),
                 bp::arg("stopCallback")=object())))
        .def("start", &WaitAnim::start, start_overloads(bp::args("bKeepAttr")))
        ;
    
    class_<ParallelAnim, boost::shared_ptr<ParallelAnim>, bases<Anim>, 
            boost::noncopyable>("ParallelAnim", no_init)
        .def(init<const std::vector<AnimPtr>&,
                optional<const object&, const object&, long long> >
                ((bp::arg("anims"), bp::arg("startCallback")=object(),
                 bp::arg("stopCallback")=object(), bp::arg("maxAge")=-1)))
        .def("start", &ParallelAnim::start, start_overloads(args("bKeepAttr")))
        ;
      
    class_<AnimState, boost::noncopyable>("AnimState", no_init)
        .def(init<const UTF8String&, AnimPtr, optional<const UTF8String&> >
                ((bp::arg("name"), bp::arg("anim"), bp::arg("nextName")="")))
        ;

    class_<StateAnim, boost::shared_ptr<StateAnim>, bases<Anim>, 
            boost::noncopyable>("StateAnim", init<const std::vector<AnimState>&>())
        .def("setState", &StateAnim::setState, setState_overloads(args("bKeepAttr")))
        .def("getState", make_function(&StateAnim::getState,
                return_value_policy<copy_const_reference>()))
        .def("setDebug", &StateAnim::setDebug) 
        ;

    def("fadeIn", fadeInDeprecated, (bp::arg("node"), bp::arg("duration"), 
            bp::arg("max")=1.0, bp::arg("stopCallback")=object()));

    def("fadeOut", fadeOutDeprecated, (bp::arg("node"), bp::arg("duration"),
            bp::arg("stopCallback")=object()));

}
