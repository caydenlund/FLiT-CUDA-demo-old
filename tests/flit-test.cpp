/**
 * @file flit-test.cpp
 * @author Cayden Lund <caydenlund@gmail.com>
 * @brief
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "flit.h"

#include <cmath>
#include <string>

using namespace std;

namespace std {
template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename...>
using void_t = void;
}  // namespace std

// Redefine main() to avoid name clash.
#define main cuda_matx_main
#include "convolution.cu"
#undef main
// Register it so we can use it in call_main() or call_mpi_main().
FLIT_REGISTER_MAIN(cuda_matx_main);

template <typename T>
class Convolution : public flit::TestBase<T> {
   public:
    Convolution(std::string id) : flit::TestBase<T>(std::move(id)) {}
    virtual size_t getInputsPerRun() override { return 0; }
    virtual std::vector<T> getDefaultInput() override { return {}; }

    virtual long double compare(
        const std::vector<std::string> &ground_truth,
        const std::vector<std::string> &test_results) const override {
        // Read base mesh
        std::istringstream gt_meshreader(ground_truth[0]);
        mfem::Mesh gt_mesh(gt_meshreader);

        std::istringstream tr_meshreader(test_results[0]);
        mfem::Mesh tr_mesh(tr_meshreader);

        // Read eigenmodes and compute error
        //   Error per mode is L2(ground_truth - test_result) / L2(ground_truth)
        //   We return the average of this
        long double nume = 0;
        long double deno = 0;
        for (int i = 1; i < 2; i++) {
            std::istringstream gt_modereader(ground_truth[i]);
            mfem::GridFunction gt_mode(&gt_mesh, gt_modereader);

            deno += gt_mode.Norml2();

            std::istringstream tr_modereader(test_results[i]);
            mfem::GridFunction tr_mode(&tr_mesh, tr_modereader);

            mfem::GridFunction diff(gt_mode);
            diff -= tr_mode;
            nume += diff.Norml2();
        }

        // return the relative error as a percent
        return 100 * nume / deno;
    }

   protected:
    virtual flit::Variant run_impl(const std::vector<T> &ti) override {
        // Empty test for the general case
        FLIT_UNUSED(ti);
        return flit::Variant();
    }

   protected:
    using flit::TestBase<T>::id;
};

// Only implement the test for double precision
template <>
flit::Variant Convolution<double>::run_impl(const std::vector<double> &ti) {
    FLIT_UNUSED(ti);

    // Run the example's main().
    auto meshfile = flit::join(start_dir, "data", "beam-tet.mesh");
    auto result =
        flit::call_mpi_main(cuda_matx_main, "mpirun -n 1 --bind-to none",
                        "Convolution", "--no-visualization --mesh " + meshfile);

    // Output debugging information
    std::ostream &out = flit::info_stream;
    out << id << " stdout: " << result.out << "\n";
    out << id << " stderr: " << result.err << "\n";
    out << id << " return: " << result.ret << "\n";
    out.flush();

    if (result.ret != 0) {
        throw std::logic_error("Failed to run main() correctly.");
    }
}

REGISTER_TYPE(Convolution)
