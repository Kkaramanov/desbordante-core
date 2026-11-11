#include "python_bindings/phm/bind_phm.h"

#include <pybind11/pybind11.h>

#include <pybind11/stl.h>

#include "core/algorithms/pp/phm/phm.h"
#include "core/algorithms/pp/phm/phui.h"
#include "python_bindings/py_util/bind_primitive.h"

namespace python_bindings {

void BindPhm(pybind11::module_& main_module) {
    using namespace algos;

    auto phm_module = main_module.def_submodule("phm");

    pybind11::class_<phm::Phui>(phm_module, "Phui")
            .def_readonly("items", &phm::Phui::items)
            .def_readonly("utility", &phm::Phui::utility)
            .def_readonly("support", &phm::Phui::support)
            .def_readonly("min_periodicity", &phm::Phui::min_periodicity)
            .def_readonly("max_periodicity", &phm::Phui::max_periodicity)
            .def_readonly("avg_periodicity", &phm::Phui::avg_periodicity)
            .def("__str__", &phm::Phui::ToString)
            .def("__hash__", [](phm::Phui const& self) { return phm::Phui::Hash{}(self); })
            .def("__eq__",
                 [](phm::Phui const& self, phm::Phui const& other) { return self == other; });

    BindPrimitiveNoBase<Phm>(phm_module, "phm").def("get_phui", &Phm::GetPhuis);
}

}  // namespace python_bindings