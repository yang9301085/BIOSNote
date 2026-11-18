## [`CL`](https://learn.microsoft.com/zh-cn/cpp/build/reference/compiling-a-c-cpp-program?view=msvc-170)  
### 使用编译器 (cl.exe) 可编译源代码文件，并将其链接到应用、库和 DLL 中。
#### CL 命令行使用以下语法：

`CL [option...] file... [option | file]... [lib...] [@command-file] [/link link-opt...]`
下表描述了 CL 命令的输入。

|条目|含义|
|---|---|
|_option_|一个或多个 [CL 选项](https://learn.microsoft.com/zh-cn/cpp/build/reference/compiler-options?view=msvc-170)。 请注意，所有选项都适用于所有指定的源文件。 选项由正斜杠 (/) 或短划线 (-) 指定。 如果某个选项采用参数，则该选项的说明会记录选项和参数之间是否允许添加空格。 选项名称（/HELP 选项除外）区分大小写。 有关详细信息，请参阅 [CL 选项的顺序](https://learn.microsoft.com/zh-cn/cpp/build/reference/order-of-cl-options?view=msvc-170)。|
|`file`|一个或多个源文件、.obj 文件或库的名称。 CL 编译源文件，并将 .obj 文件和库的名称传递给链接器。 有关详细信息，请参阅 [CL 文件名语法](https://learn.microsoft.com/zh-cn/cpp/build/reference/cl-filename-syntax?view=msvc-170)。|
|lib|一个或多个库名称。 CL 将这些名称传递给链接器。|
|command-file|包含多个选项和文件名的文件。 有关详细信息，请参阅 [CL 命令文件](https://learn.microsoft.com/zh-cn/cpp/build/reference/cl-command-files?view=msvc-170)。|
|link-opt|一个或多个 [MSVC 链接器选项](https://learn.microsoft.com/zh-cn/cpp/build/reference/linker-options?view=msvc-170)。 CL 将这些选项传递给链接器。|

可以指定任意数量的选项、文件名和库名称，前提是命令行中的字符数不超过 1024，即操作系统规定的限制。
##### CL选项的顺序

选项可以出现在 CL 命令行上的任何位置，但 /link 选项除外，此选项必须出现在最后。 编译器从 [CL 环境变量](https://learn.microsoft.com/zh-cn/cpp/build/reference/cl-environment-variables?view=msvc-170)中指定的选项开始，然后从左到右读取命令行，按照遇到的顺序处理命令文件。 每个选项都适用于命令行中的所有文件。 如果 CL 遇到冲突的选项，它会使用最右边的选项。

##### Compiler options(listed alphabetically)

|Option|Purpose|
|---|---|
|[`@`](https://learn.microsoft.com/en-us/cpp/build/reference/at-specify-a-compiler-response-file?view=msvc-170)|Specifies a response file.|
|[`/?`](https://learn.microsoft.com/en-us/cpp/build/reference/help-compiler-command-line-help?view=msvc-170)|Lists the compiler options.|
|[`/AI<dir>`](https://learn.microsoft.com/en-us/cpp/build/reference/ai-specify-metadata-directories?view=msvc-170)|Specifies a directory to search to resolve file references passed to the [`#using`](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-using-directive-cpp?view=msvc-170) directive.|
|[`/analyze`](https://learn.microsoft.com/en-us/cpp/build/reference/analyze-code-analysis?view=msvc-170)|Enables code analysis.|
|[`/arch:<IA32\|SSE\|SSE2\|AVX\|AVX2\|AVX512>`](https://learn.microsoft.com/en-us/cpp/build/reference/arch-x86?view=msvc-170)|Minimum CPU architecture requirements. IA32, SSE, and SSE2 are x86 only.|
|`/arm64EC`|Generate code compatible with the arm64EC ABI.|
|[`/await`](https://learn.microsoft.com/en-us/cpp/build/reference/await-enable-coroutine-support?view=msvc-170)|Enable coroutines (resumable functions) extensions.|
|[`/await:strict`](https://learn.microsoft.com/en-us/cpp/build/reference/await-enable-coroutine-support?view=msvc-170)|Enable standard C++20 coroutine support with earlier language versions.|
|[`/bigobj`](https://learn.microsoft.com/en-us/cpp/build/reference/bigobj-increase-number-of-sections-in-dot-obj-file?view=msvc-170)|Increases the number of addressable sections in an .obj file.|
|[`/C`](https://learn.microsoft.com/en-us/cpp/build/reference/c-preserve-comments-during-preprocessing?view=msvc-170)|Preserves comments during preprocessing.|
|[`/c`](https://learn.microsoft.com/en-us/cpp/build/reference/c-compile-without-linking?view=msvc-170)|Compiles without linking.|
|[`/cgthreads`](https://learn.microsoft.com/en-us/cpp/build/reference/cgthreads-code-generation-threads?view=msvc-170)|Specifies number of _cl.exe_ threads to use for optimization and code generation.|
|[`/clr`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produces an output file to run on the common language runtime.|
|[`/clr:implicitKeepAlive-`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Turn off implicit emission of `System::GC::KeepAlive(this)`.|
|[`/clr:initialAppDomain`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Enable initial AppDomain behavior of Visual C++ 2002.|
|[`/clr:netcore`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produce assemblies targeting .NET Core runtime.|
|[`/clr:noAssembly`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Don't produce an assembly.|
|[`/clr:nostdimport`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Don't import any required assemblies implicitly.|
|[`/clr:nostdlib`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Ignore the system .NET framework directory when searching for assemblies.|
|[`/clr:pure`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produce an IL-only output file (no native executable code).|
|[`/clr:safe`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produce an IL-only verifiable output file.|
|[`/constexpr:backtrace<N>`](https://learn.microsoft.com/en-us/cpp/build/reference/constexpr-control-constexpr-evaluation?view=msvc-170)|Show N `constexpr` evaluations in diagnostics (default: 10).|
|[`/constexpr:depth<N>`](https://learn.microsoft.com/en-us/cpp/build/reference/constexpr-control-constexpr-evaluation?view=msvc-170)|Recursion depth limit for `constexpr` evaluation (default: 512).|
|[`/constexpr:steps<N>`](https://learn.microsoft.com/en-us/cpp/build/reference/constexpr-control-constexpr-evaluation?view=msvc-170)|Terminate `constexpr` evaluation after N steps (default: 100000)|
|[`/D<name>{=\|#}<text>`](https://learn.microsoft.com/en-us/cpp/build/reference/d-preprocessor-definitions?view=msvc-170)|Defines constants and macros.|
|[`/diagnostics`](https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options?view=msvc-170)|Diagnostics format: prints column information.|
|[`/diagnostics:caret[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options?view=msvc-170)|Diagnostics format: prints column and the indicated line of source.|
|[`/diagnostics:classic`](https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options?view=msvc-170)|Use legacy diagnostics format.|
|[`/doc`](https://learn.microsoft.com/en-us/cpp/build/reference/doc-process-documentation-comments-c-cpp?view=msvc-170)|Processes documentation comments to an XML file.|
|[`/E`](https://learn.microsoft.com/en-us/cpp/build/reference/e-preprocess-to-stdout?view=msvc-170)|Copies preprocessor output to standard output.|
|[`/EHa`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|Enable C++ exception handling (with SEH exceptions).|
|[`/EHc`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|`extern "C"` defaults to `nothrow`.|
|[`/EHr`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|Always generate `noexcept` runtime termination checks.|
|[`/EHs`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|Enable C++ exception handling (no SEH exceptions).|
|[`/EP`](https://learn.microsoft.com/en-us/cpp/build/reference/ep-preprocess-to-stdout-without-hash-line-directives?view=msvc-170)|Copies preprocessor output to standard output.|
|[`/errorReport`](https://learn.microsoft.com/en-us/cpp/build/reference/errorreport-report-internal-compiler-errors?view=msvc-170)|Deprecated. [Windows Error Reporting (WER)](https://learn.microsoft.com/en-us/windows/win32/wer/windows-error-reporting) settings control error reporting.|
|[`/execution-charset`](https://learn.microsoft.com/en-us/cpp/build/reference/execution-charset-set-execution-character-set?view=msvc-170)|Set execution character set.|
|[`/experimental:log`](https://learn.microsoft.com/en-us/cpp/build/reference/experimental-log?view=msvc-170)|Enables experimental structured SARIF output.|
|[`/experimental:module`](https://learn.microsoft.com/en-us/cpp/build/reference/experimental-module?view=msvc-170)|Enables experimental module support.|
|[`/exportHeader`](https://learn.microsoft.com/en-us/cpp/build/reference/module-exportheader?view=msvc-170)|Create the header units files (_`.ifc`_) specified by the input arguments.|
|[`/external:anglebrackets`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Treat all headers included via `<>` as external.|
|[`/external:env:<var>`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Specify an environment variable with locations of external headers.|
|[`/external:I <path>`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Specify location of external headers.|
|[`/external:templates[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Evaluate warning level across template instantiation chain.|
|[`/external:W<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Set warning level for external headers.|
|[`/F`](https://learn.microsoft.com/en-us/cpp/build/reference/f-set-stack-size?view=msvc-170)|Sets stack size.|
|[`/FA`](https://learn.microsoft.com/en-us/cpp/build/reference/fa-fa-listing-file?view=msvc-170)|Configures an assembly listing file.|
|[`/Fa`](https://learn.microsoft.com/en-us/cpp/build/reference/fa-fa-listing-file?view=msvc-170)|Creates an assembly listing file.|
|`/fastfail`|Enable fast-fail mode.|
|[`/favor:<blend\|AMD64\|INTEL64\|ATOM>`](https://learn.microsoft.com/en-us/cpp/build/reference/favor-optimize-for-architecture-specifics?view=msvc-170)|Produces code that is optimized for a specified architecture, or for a range of architectures.|
|[`/FC`](https://learn.microsoft.com/en-us/cpp/build/reference/fc-full-path-of-source-code-file-in-diagnostics?view=msvc-170)|Displays the full path of source code files passed to _cl.exe_ in diagnostic text.|
|[`/Fd`](https://learn.microsoft.com/en-us/cpp/build/reference/fd-program-database-file-name?view=msvc-170)|Renames program database file.|
|[`/Fe`](https://learn.microsoft.com/en-us/cpp/build/reference/fe-name-exe-file?view=msvc-170)|Renames the executable file.|
|[`/FI<file>`](https://learn.microsoft.com/en-us/cpp/build/reference/fi-name-forced-include-file?view=msvc-170)|Preprocesses the specified include file.|
|[`/Fi`](https://learn.microsoft.com/en-us/cpp/build/reference/fi-preprocess-output-file-name?view=msvc-170)|Specifies the preprocessed output file name.|
|[`/Fm`](https://learn.microsoft.com/en-us/cpp/build/reference/fm-name-mapfile?view=msvc-170)|Creates a mapfile.|
|[`/Fo`](https://learn.microsoft.com/en-us/cpp/build/reference/fo-object-file-name?view=msvc-170)|Creates an object file.|
|[`/Fp`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-name-dot-pch-file?view=msvc-170)|Specifies a precompiled header file name.|
|[`/fp:contract`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|Consider floating-point contractions when generating code.|
|[`/fp:except[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|Consider floating-point exceptions when generating code.|
|[`/fp:fast`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|"fast" floating-point model; results are less predictable.|
|[`/fp:precise`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|"precise" floating-point model; results are predictable.|
|[`/fp:strict`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|"strict" floating-point model (implies `/fp:except`).|
|[`/fpcvt:BC`](https://learn.microsoft.com/en-us/cpp/build/reference/fpcvt?view=msvc-170)|Backward-compatible floating-point to unsigned integer conversions.|
|[`/fpcvt:IA`](https://learn.microsoft.com/en-us/cpp/build/reference/fpcvt?view=msvc-170)|Intel native floating-point to unsigned integer conversion behavior.|
|[`/FR`, `/Fr`](https://learn.microsoft.com/en-us/cpp/build/reference/fr-fr-create-dot-sbr-file?view=msvc-170)|Name generated _`.sbr`_ browser files. **`/Fr`** is deprecated.|
|[`/FS`](https://learn.microsoft.com/en-us/cpp/build/reference/fs-force-synchronous-pdb-writes?view=msvc-170)|Forces writes to the PDB file to be serialized through _MSPDBSRV.EXE_.|
|[`/fsanitize`](https://learn.microsoft.com/en-us/cpp/build/reference/fsanitize?view=msvc-170)|Enables compilation of sanitizer instrumentation such as AddressSanitizer.|
|[`/fsanitize-coverage`](https://learn.microsoft.com/en-us/cpp/build/reference/fsanitize-coverage?view=msvc-170)|Enables compilation of code coverage instrumentation for libraries such as LibFuzzer.|
|`/Ft<dir>`|Location of the header files generated for `#import`.|
|[`/FU<file>`](https://learn.microsoft.com/en-us/cpp/build/reference/fu-name-forced-hash-using-file?view=msvc-170)|Forces the use of a file name, as if it had been passed to the [`#using`](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-using-directive-cpp?view=msvc-170) directive.|
|[`/Fx`](https://learn.microsoft.com/en-us/cpp/build/reference/fx-merge-injected-code?view=msvc-170)|Merges injected code with the source file.|
|[`/GA`](https://learn.microsoft.com/en-us/cpp/build/reference/ga-optimize-for-windows-application?view=msvc-170)|Optimizes for Windows applications.|
|[`/Gd`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__cdecl`** calling convention. (x86 only)|
|[`/Ge`](https://learn.microsoft.com/en-us/cpp/build/reference/ge-enable-stack-probes?view=msvc-170)|Deprecated. Activates stack probes.|
|[`/GF`](https://learn.microsoft.com/en-us/cpp/build/reference/gf-eliminate-duplicate-strings?view=msvc-170)|Enables string pooling.|
|[`/GH`](https://learn.microsoft.com/en-us/cpp/build/reference/gh-enable-pexit-hook-function?view=msvc-170)|Calls hook function `_pexit`.|
|[`/Gh`](https://learn.microsoft.com/en-us/cpp/build/reference/gh-enable-penter-hook-function?view=msvc-170)|Calls hook function `_penter`.|
|[`/GL[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gl-whole-program-optimization?view=msvc-170)|Enables whole program optimization.|
|[`/Gm[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gm-enable-minimal-rebuild?view=msvc-170)|Deprecated. Enables minimal rebuild.|
|[`/GR[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gr-enable-run-time-type-information?view=msvc-170)|Enables run-time type information (RTTI).|
|[`/Gr`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__fastcall`** calling convention. (x86 only)|
|[`/GS[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check?view=msvc-170)|Checks buffer security.|
|[`/Gs[n]`](https://learn.microsoft.com/en-us/cpp/build/reference/gs-control-stack-checking-calls?view=msvc-170)|Controls stack probes.|
|[`/GT`](https://learn.microsoft.com/en-us/cpp/build/reference/gt-support-fiber-safe-thread-local-storage?view=msvc-170)|Supports fiber safety for data allocated by using static thread-local storage.|
|`/Gu[-]`|Ensure distinct functions have distinct addresses.|
|[`/guard:cf[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/guard-enable-control-flow-guard?view=msvc-170)|Adds control flow guard security checks.|
|[`/guard:ehcont[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/guard-enable-eh-continuation-metadata?view=msvc-170)|Enables EH continuation metadata.|
|[`/Gv`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__vectorcall`** calling convention. (x86 and x64 only)|
|[`/Gw[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gw-optimize-global-data?view=msvc-170)|Enables whole-program global data optimization.|
|[`/GX[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gx-enable-exception-handling?view=msvc-170)|Deprecated. Enables synchronous exception handling. Use [`/EH`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170) instead.|
|[`/Gy[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gy-enable-function-level-linking?view=msvc-170)|Enables function-level linking.|
|[`/GZ`](https://learn.microsoft.com/en-us/cpp/build/reference/gz-enable-stack-frame-run-time-error-checking?view=msvc-170)|Deprecated. Enables fast checks. (Same as [`/RTC1`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170))|
|[`/Gz`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__stdcall`** calling convention. (x86 only)|
|[`/H`](https://learn.microsoft.com/en-us/cpp/build/reference/h-restrict-length-of-external-names?view=msvc-170)|Deprecated. Restricts the length of external (public) names.|
|[`/headerName`](https://learn.microsoft.com/en-us/cpp/build/reference/headername?view=msvc-170)|Build a header unit from the specified header.|
|[`/headerUnit`](https://learn.microsoft.com/en-us/cpp/build/reference/headerunit?view=msvc-170)|Specify where to find the header unit file (`.ifc`) for the specified header.|
|[`/HELP`](https://learn.microsoft.com/en-us/cpp/build/reference/help-compiler-command-line-help?view=msvc-170)|Lists the compiler options.|
|[`/homeparams`](https://learn.microsoft.com/en-us/cpp/build/reference/homeparams-copy-register-parameters-to-stack?view=msvc-170)|Forces parameters passed in registers to be written to their locations on the stack upon function entry. This compiler option is only for the x64 compilers (native and cross compile).|
|[`/hotpatch`](https://learn.microsoft.com/en-us/cpp/build/reference/hotpatch-create-hotpatchable-image?view=msvc-170)|Creates a hotpatchable image.|
|[`/I<dir>`](https://learn.microsoft.com/en-us/cpp/build/reference/i-additional-include-directories?view=msvc-170)|Searches a directory for include files.|
|[`/ifcOutput`](https://learn.microsoft.com/en-us/cpp/build/reference/ifc-output?view=msvc-170)|Specify output file name or directory for built _`.ifc`_ files.|
|[`/interface`](https://learn.microsoft.com/en-us/cpp/build/reference/interface?view=msvc-170)|Treat the input file as a module interface unit.|
|[`/internalPartition`](https://learn.microsoft.com/en-us/cpp/build/reference/internal-partition?view=msvc-170)|Treat the input file as an internal partition unit.|
|[`/J`](https://learn.microsoft.com/en-us/cpp/build/reference/j-default-char-type-is-unsigned?view=msvc-170)|Changes the default **`char`** type.|
|[`/jumptablerdata`](https://learn.microsoft.com/en-us/cpp/build/reference/jump-table-rdata?view=msvc-170)|Put switch case statement jump tables in the `.rdata` section.|
|[`/JMC`](https://learn.microsoft.com/en-us/cpp/build/reference/jmc?view=msvc-170)|Supports native C++ Just My Code debugging.|
|[`/kernel`](https://learn.microsoft.com/en-us/cpp/build/reference/kernel-create-kernel-mode-binary?view=msvc-170)|The compiler and linker create a binary that can be executed in the Windows kernel.|
|[`/LD`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Creates a dynamic-link library.|
|[`/LDd`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Creates a debug dynamic-link library.|
|[`/link`](https://learn.microsoft.com/en-us/cpp/build/reference/link-pass-options-to-linker?view=msvc-170)|Passes the specified option to LINK.|
|[`/LN`](https://learn.microsoft.com/en-us/cpp/build/reference/ln-create-msil-module?view=msvc-170)|Creates an MSIL `.netmodule`.|
|[`/MD`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a multithreaded DLL, by using _MSVCRT.lib_.|
|[`/MDd`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a debug multithreaded DLL, by using _MSVCRTD.lib_.|
|[`/MP`](https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes?view=msvc-170)|Builds multiple source files concurrently.|
|[`/MT`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a multithreaded executable file, by using _LIBCMT.lib_.|
|[`/MTd`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a debug multithreaded executable file, by using _LIBCMTD.lib_.|
|[`/nologo`](https://learn.microsoft.com/en-us/cpp/build/reference/nologo-suppress-startup-banner-c-cpp?view=msvc-170)|Suppresses display of sign-on banner.|
|[`/O1`](https://learn.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed?view=msvc-170)|Creates small code.|
|[`/O2`](https://learn.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed?view=msvc-170)|Creates fast code.|
|[`/Ob<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/ob-inline-function-expansion?view=msvc-170)|Controls inline expansion.|
|[`/Od`](https://learn.microsoft.com/en-us/cpp/build/reference/od-disable-debug?view=msvc-170)|Disables optimization.|
|[`/Og`](https://learn.microsoft.com/en-us/cpp/build/reference/og-global-optimizations?view=msvc-170)|Deprecated. Uses global optimizations.|
|[`/Oi[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/oi-generate-intrinsic-functions?view=msvc-170)|Generates intrinsic functions.|
|[`/openmp`](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=msvc-170)|Enables [`#pragma omp`](https://learn.microsoft.com/en-us/cpp/preprocessor/omp?view=msvc-170) in source code.|
|[`/openmp:experimental`](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=msvc-170)|Enable OpenMP 2.0 language extensions plus select OpenMP 3.0+ language extensions.|
|[`/openmp:llvm`](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=msvc-170)|OpenMP language extensions using LLVM runtime.|
|[`/options:strict`](https://learn.microsoft.com/en-us/cpp/build/reference/options-strict?view=msvc-170)|Unrecognized compiler options are errors.|
|[`/Os`](https://learn.microsoft.com/en-us/cpp/build/reference/os-ot-favor-small-code-favor-fast-code?view=msvc-170)|Favors small code.|
|[`/Ot`](https://learn.microsoft.com/en-us/cpp/build/reference/os-ot-favor-small-code-favor-fast-code?view=msvc-170)|Favors fast code.|
|[`/Ox`](https://learn.microsoft.com/en-us/cpp/build/reference/ox-full-optimization?view=msvc-170)|A subset of /O2 that doesn't include /GF or /Gy.|
|[`/Oy`](https://learn.microsoft.com/en-us/cpp/build/reference/oy-frame-pointer-omission?view=msvc-170)|Omits frame pointer. (x86 only)|
|[`/P`](https://learn.microsoft.com/en-us/cpp/build/reference/p-preprocess-to-a-file?view=msvc-170)|Writes preprocessor output to a file.|
|`/PD`|Print all macro definitions.|
|[`/permissive[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance?view=msvc-170)|Set standard-conformance mode.|
|`/PH`|Generate `#pragma file_hash` when preprocessing.|
|`/presetPadding`|Zero initialize padding for stack based class types.|
|[`/Qfast_transcendentals`](https://learn.microsoft.com/en-us/cpp/build/reference/qfast-transcendentals-force-fast-transcendentals?view=msvc-170)|Generates fast transcendentals.|
|[`/QIfist`](https://learn.microsoft.com/en-us/cpp/build/reference/qifist-suppress-ftol?view=msvc-170)|Deprecated. Suppresses the call of the helper function `_ftol` when a conversion from a floating-point type to an integral type is required. (x86 only)|
|[`/Qimprecise_fwaits`](https://learn.microsoft.com/en-us/cpp/build/reference/qimprecise-fwaits-remove-fwaits-inside-try-blocks?view=msvc-170)|Removes `fwait` commands inside **`try`** blocks.|
|[`/QIntel-jcc-erratum`](https://learn.microsoft.com/en-us/cpp/build/reference/qintel-jcc-erratum?view=msvc-170)|Mitigates the performance impact of the Intel JCC erratum microcode update.|
|[`/Qpar-report:<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/qpar-report-auto-parallelizer-reporting-level?view=msvc-170)|Enables reporting levels for automatic parallelization.|
|[`/Qpar`](https://learn.microsoft.com/en-us/cpp/build/reference/qpar-auto-parallelizer?view=msvc-170)|Enables automatic parallelization of loops.|
|[`/Qsafe_fp_loads`](https://learn.microsoft.com/en-us/cpp/build/reference/qsafe-fp-loads?view=msvc-170)|Uses integer move instructions for floating-point values and disables certain floating point load optimizations.|
|[`/Qspectre[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/qspectre?view=msvc-170)|Enable mitigations for CVE 2017-5753, for a class of Spectre attacks.|
|[`/Qspectre-load`](https://learn.microsoft.com/en-us/cpp/build/reference/qspectre-load?view=msvc-170)|Generate serializing instructions for every load instruction.|
|[`/Qspectre-load-cf`](https://learn.microsoft.com/en-us/cpp/build/reference/qspectre-load-cf?view=msvc-170)|Generate serializing instructions for every control flow instruction that loads memory.|
|[`/Qvec-report:<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/qvec-report-auto-vectorizer-reporting-level?view=msvc-170)|Enables reporting levels for automatic vectorization.|
|[`/reference`](https://learn.microsoft.com/en-us/cpp/build/reference/module-reference?view=msvc-170)|Use named module IFC.|
|[`/RTC1`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Enable fast runtime checks (equivalent to `/RTCsu`).|
|[`/RTCc`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Convert to smaller type checks at run-time.|
|[`/RTCs`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Enable stack frame runtime checks.|
|[`/RTCu`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Enables uninitialized local usage checks.|
|[`/scanDependencies`](https://learn.microsoft.com/en-us/cpp/build/reference/scandependencies?view=msvc-170)|List module dependencies in C++ Standard JSON form.|
|[`/sdl`](https://learn.microsoft.com/en-us/cpp/build/reference/sdl-enable-additional-security-checks?view=msvc-170)|Enable more security features and warnings.|
|[`/showIncludes`](https://learn.microsoft.com/en-us/cpp/build/reference/showincludes-list-include-files?view=msvc-170)|Displays a list of all include files during compilation.|
|[`/source-charset`](https://learn.microsoft.com/en-us/cpp/build/reference/source-charset-set-source-character-set?view=msvc-170)|Set source character set.|
|[`/sourceDependencies`](https://learn.microsoft.com/en-us/cpp/build/reference/sourcedependencies?view=msvc-170)|List all source-level dependencies.|
|[`/sourceDependencies:directives`](https://learn.microsoft.com/en-us/cpp/build/reference/sourcedependencies-directives?view=msvc-170)|List module and header unit dependencies.|
|[`/std:c++14`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C++14 standard ISO/IEC 14882:2014 (default).|
|[`/std:c++17`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C++17 standard ISO/IEC 14882:2017.|
|[`/std:c++20`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C++20 standard ISO/IEC 14882:2020.|
|[`/std:c++latest`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|The latest draft C++ standard preview features.|
|[`/std:c11`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C11 standard ISO/IEC 9899:2011.|
|[`/std:c17`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C17 standard ISO/IEC 9899:2018.|
|[`/TC`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies all source files are C.|
|[`/Tc`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies a C source file.|
|[`/TP`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies all source files are C++.|
|[`/Tp`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies a C++ source file.|
|[`/translateInclude`](https://learn.microsoft.com/en-us/cpp/build/reference/translateinclude?view=msvc-170)|Treat `#include` as `import`.|
|[`/U<name>`](https://learn.microsoft.com/en-us/cpp/build/reference/u-u-undefine-symbols?view=msvc-170)|Removes a predefined macro.|
|[`/u`](https://learn.microsoft.com/en-us/cpp/build/reference/u-u-undefine-symbols?view=msvc-170)|Removes all predefined macros.|
|[`/utf-8`](https://learn.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=msvc-170)|Set source and execution character sets to UTF-8.|
|[`/V`](https://learn.microsoft.com/en-us/cpp/build/reference/v-version-number?view=msvc-170)|Deprecated. Sets the version string.|
|[`/validate-charset`](https://learn.microsoft.com/en-us/cpp/build/reference/validate-charset-validate-for-compatible-characters?view=msvc-170)|Validate UTF-8 files for only compatible characters.|
|[`/vd{0\|1\|2}`](https://learn.microsoft.com/en-us/cpp/build/reference/vd-disable-construction-displacements?view=msvc-170)|Suppresses or enables hidden `vtordisp` class members.|
|[`/vmb`](https://learn.microsoft.com/en-us/cpp/build/reference/vmb-vmg-representation-method?view=msvc-170)|Uses best base for pointers to members.|
|[`/vmg`](https://learn.microsoft.com/en-us/cpp/build/reference/vmb-vmg-representation-method?view=msvc-170)|Uses full generality for pointers to members.|
|[`/vmm`](https://learn.microsoft.com/en-us/cpp/build/reference/vmm-vms-vmv-general-purpose-representation?view=msvc-170)|Declares multiple inheritance.|
|[`/vms`](https://learn.microsoft.com/en-us/cpp/build/reference/vmm-vms-vmv-general-purpose-representation?view=msvc-170)|Declares single inheritance.|
|[`/vmv`](https://learn.microsoft.com/en-us/cpp/build/reference/vmm-vms-vmv-general-purpose-representation?view=msvc-170)|Declares virtual inheritance.|
|[`/volatile:iso`](https://learn.microsoft.com/en-us/cpp/build/reference/volatile-volatile-keyword-interpretation?view=msvc-170)|Acquire/release semantics not guaranteed on volatile accesses.|
|[`/volatile:ms`](https://learn.microsoft.com/en-us/cpp/build/reference/volatile-volatile-keyword-interpretation?view=msvc-170)|Acquire/release semantics guaranteed on volatile accesses.|
|`/volatileMetadata`|Generate metadata on volatile memory accesses.|
|[`/w`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Disable all warnings.|
|[`/W0`, `/W1`, `/W2`, `/W3`, `/W4`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Set output warning level.|
|[`/w1<n>`, `/w2<n>`, `/w3<n>`, `/w4<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Set warning level for the specified warning.|
|[`/Wall`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Enable all warnings, including warnings that are disabled by default.|
|[`/wd<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Disable the specified warning.|
|[`/we<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Treat the specified warning as an error.|
|[`/WL`](https://learn.microsoft.com/en-us/cpp/build/reference/wl-enable-one-line-diagnostics?view=msvc-170)|Enable one-line diagnostics for error and warning messages when compiling C++ source code from the command line.|
|[`/wo<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Display the specified warning only once.|
|[`/Wv:xx[.yy[.zzzzz]]`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Disable warnings introduced after the specified version of the compiler.|
|[`/WX`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Treat warnings as errors.|
|[`/X`](https://learn.microsoft.com/en-us/cpp/build/reference/x-ignore-standard-include-paths?view=msvc-170)|Ignores the standard include directory.|
|[`/Y-`](https://learn.microsoft.com/en-us/cpp/build/reference/y-ignore-precompiled-header-options?view=msvc-170)|Ignores all other precompiled-header compiler options in the current build.|
|[`/Yc`](https://learn.microsoft.com/en-us/cpp/build/reference/yc-create-precompiled-header-file?view=msvc-170)|Create _`.PCH`_ file.|
|[`/Yd`](https://learn.microsoft.com/en-us/cpp/build/reference/yd-place-debug-information-in-object-file?view=msvc-170)|Deprecated. Places complete debugging information in all object files. Use [`/Zi`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170) instead.|
|[`/Yl`](https://learn.microsoft.com/en-us/cpp/build/reference/yl-inject-pch-reference-for-debug-library?view=msvc-170)|Injects a PCH reference when creating a debug library.|
|[`/Yu`](https://learn.microsoft.com/en-us/cpp/build/reference/yu-use-precompiled-header-file?view=msvc-170)|Uses a precompiled header file during build.|
|[`/Z7`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170)|Generates C 7.0-compatible debugging information.|
|[`/Za`](https://learn.microsoft.com/en-us/cpp/build/reference/za-ze-disable-language-extensions?view=msvc-170)|Disables some C89 language extensions in C code.|
|[`/Zc:__cplusplus[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-170)|Enable the `__cplusplus` macro to report the supported standard (off by default).|
|[`/Zc:__STDC__`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-stdc?view=msvc-170)|Enable the `__STDC__` macro to report the C standard is supported (off by default).|
|[`/Zc:alignedNew[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-alignednew?view=msvc-170)|Enable C++17 over-aligned dynamic allocation (on by default in C++17).|
|[`/Zc:auto[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-auto-deduce-variable-type?view=msvc-170)|Enforce the new Standard C++ meaning for **`auto`** (on by default).|
|[`/Zc:char8_t[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-char8-t?view=msvc-170)|Enable or disable C++20 native `u8` literal support as `const char8_t` (off by default, except under **`/std:c++20`**).|
|[`/Zc:enumTypes[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-enumtypes?view=msvc-170)|Enable Standard C++ rules for `enum` type deduction (off by default).|
|[`/Zc:externC[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-externc?view=msvc-170)|Enforce Standard C++ rules for `extern "C"` functions (implied by **`/permissive-`**).|
|[`/Zc:externConstexpr[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-externconstexpr?view=msvc-170)|Enable external linkage for **`constexpr`** variables (off by default).|
|[`/Zc:forScope[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-forscope-force-conformance-in-for-loop-scope?view=msvc-170)|Enforce Standard C++ **`for`** scoping rules (on by default).|
|[`/Zc:gotoScope`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-gotoscope?view=msvc-170)|Enforce Standard C++ **`goto`** rules around local variable initialization (implied by **`/permissive-`**).|
|[`/Zc:hiddenFriend[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-hiddenfriend?view=msvc-170)|Enforce Standard C++ hidden friend rules (implied by **`/permissive-`**)|
|[`/Zc:implicitNoexcept[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-implicitnoexcept-implicit-exception-specifiers?view=msvc-170)|Enable implicit **`noexcept`** on required functions (on by default).|
|[`/Zc:inline[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-inline-remove-unreferenced-comdat?view=msvc-170)|Remove unreferenced functions or data if they're COMDAT or have internal linkage only (off by default).|
|[`/Zc:lambda[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-lambda?view=msvc-170)|Enable new lambda processor for conformance-mode syntactic checks in generic lambdas.|
|[`/Zc:noexceptTypes[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-noexcepttypes?view=msvc-170)|Enforce C++17 **`noexcept`** rules (on by default in C++17 or later).|
|[`/Zc:nrvo[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-nrvo?view=msvc-170)|Enable optional copy and move elisions (on by default under **`/O2`**, **`/permissive-`**, or **`/std:c++20`** or later).|
|[`/Zc:preprocessor[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-preprocessor?view=msvc-170)|Use the new conforming preprocessor (off by default, except in C11/C17).|
|[`/Zc:referenceBinding[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-referencebinding-enforce-reference-binding-rules?view=msvc-170)|A UDT temporary won't bind to a non-const lvalue reference (off by default).|
|[`/Zc:rvalueCast[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-rvaluecast-enforce-type-conversion-rules?view=msvc-170)|Enforce Standard C++ explicit type conversion rules (off by default).|
|[`/Zc:sizedDealloc[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-sizeddealloc-enable-global-sized-dealloc-functions?view=msvc-170)|Enable C++14 global sized deallocation functions (on by default).|
|[`/Zc:strictStrings[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-strictstrings-disable-string-literal-type-conversion?view=msvc-170)|Disable string-literal to `char*` or `wchar_t*` conversion (off by default).|
|[`/Zc:templateScope[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-templatescope?view=msvc-170)|Enforce Standard C++ template parameter shadowing rules (off by default).|
|[`/Zc:ternary[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-ternary?view=msvc-170)|Enforce conditional operator rules on operand types (off by default).|
|[`/Zc:threadSafeInit[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-threadsafeinit-thread-safe-local-static-initialization?view=msvc-170)|Enable thread-safe local static initialization (on by default).|
|[`/Zc:throwingNew[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-throwingnew-assume-operator-new-throws?view=msvc-170)|Assume **`operator new`** throws on failure (off by default).|
|[`/Zc:tlsGuards[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-tlsguards?view=msvc-170)|Generate runtime checks for TLS variable initialization (on by default).|
|[`/Zc:trigraphs`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-trigraphs-trigraphs-substitution?view=msvc-170)|Enable trigraphs (obsolete, off by default).|
|[`/Zc:twoPhase[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-twophase?view=msvc-170)|Use nonconforming template parsing behavior (conforming by default).|
|[`/Zc:wchar_t[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-wchar-t-wchar-t-is-native-type?view=msvc-170)|**`wchar_t`** is a native type, not a typedef (on by default).|
|[`/Zc:zeroSizeArrayNew[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-zerosizearraynew?view=msvc-170)|Call member `new`/`delete` for zero-size arrays of objects (on by default).|
|[`/Ze`](https://learn.microsoft.com/en-us/cpp/build/reference/za-ze-disable-language-extensions?view=msvc-170)|Deprecated. Enables C89 language extensions.|
|[`/Zf`](https://learn.microsoft.com/en-us/cpp/build/reference/zf?view=msvc-170)|Improves PDB generation time in parallel builds.|
|[`/ZH:[MD5\|SHA1\|SHA_256]`](https://learn.microsoft.com/en-us/cpp/build/reference/zh?view=msvc-170)|Specifies MD5, SHA-1, or SHA-256 for checksums in debug info.|
|[`/ZI`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170)|Includes debug information in a program database compatible with Edit and Continue. (x86 only)|
|[`/Zi`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170)|Generates complete debugging information.|
|[`/Zl`](https://learn.microsoft.com/en-us/cpp/build/reference/zl-omit-default-library-name?view=msvc-170)|Removes the default library name from the _`.obj`_ file.|
|[`/Zm`](https://learn.microsoft.com/en-us/cpp/build/reference/zm-specify-precompiled-header-memory-allocation-limit?view=msvc-170)|Specifies the precompiled header memory allocation limit.|
|[`/Zo[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zo-enhance-optimized-debugging?view=msvc-170)|Generate richer debugging information for optimized code.|
|[`/Zp[n]`](https://learn.microsoft.com/en-us/cpp/build/reference/zp-struct-member-alignment?view=msvc-170)|Packs structure members.|
|[`/Zs`](https://learn.microsoft.com/en-us/cpp/build/reference/zs-syntax-check-only?view=msvc-170)|Checks syntax only.|
|[`/ZW`](https://learn.microsoft.com/en-us/cpp/build/reference/zw-windows-runtime-compilation?view=msvc-170)|Produces an output file to run on the Windows Runtime.|

##### Compiler options (listed by category)
###### Optimization

|Option|Purpose|
|---|---|
|[`/favor:<blend\|AMD64\|INTEL64\|ATOM>`](https://learn.microsoft.com/en-us/cpp/build/reference/favor-optimize-for-architecture-specifics?view=msvc-170)|Produces code that is optimized for a specified architecture, or for a range of architectures.|
|[`/O1`](https://learn.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed?view=msvc-170)|Creates small code.|
|[`/O2`](https://learn.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed?view=msvc-170)|Creates fast code.|
|[`/Ob<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/ob-inline-function-expansion?view=msvc-170)|Controls inline expansion.|
|[`/Od`](https://learn.microsoft.com/en-us/cpp/build/reference/od-disable-debug?view=msvc-170)|Disables optimization.|
|[`/Og`](https://learn.microsoft.com/en-us/cpp/build/reference/og-global-optimizations?view=msvc-170)|Deprecated. Uses global optimizations.|
|[`/Oi[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/oi-generate-intrinsic-functions?view=msvc-170)|Generates intrinsic functions.|
|[`/Os`](https://learn.microsoft.com/en-us/cpp/build/reference/os-ot-favor-small-code-favor-fast-code?view=msvc-170)|Favors small code.|
|[`/Ot`](https://learn.microsoft.com/en-us/cpp/build/reference/os-ot-favor-small-code-favor-fast-code?view=msvc-170)|Favors fast code.|
|[`/Ox`](https://learn.microsoft.com/en-us/cpp/build/reference/ox-full-optimization?view=msvc-170)|A subset of /O2 that doesn't include /GF or /Gy.|
|[`/Oy`](https://learn.microsoft.com/en-us/cpp/build/reference/oy-frame-pointer-omission?view=msvc-170)|Omits frame pointer. (x86 only)|

###### Code generation

|Option|Purpose|
|---|---|
|[`/arch:<IA32\|SSE\|SSE2\|AVX\|AVX2\|AVX512>`](https://learn.microsoft.com/en-us/cpp/build/reference/arch-x86?view=msvc-170)|Minimum CPU architecture requirements. IA32, SSE, and SSE2 are x86 only.|
|[`/clr`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produces an output file to run on the common language runtime.|
|[`/clr:implicitKeepAlive-`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Turn off implicit emission of `System::GC::KeepAlive(this)`.|
|[`/clr:initialAppDomain`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Enable initial AppDomain behavior of Visual C++ 2002.|
|[`/clr:netcore`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produce assemblies targeting .NET Core runtime.|
|[`/clr:noAssembly`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Don't produce an assembly.|
|[`/clr:nostdimport`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Don't import any required assemblies implicitly.|
|[`/clr:nostdlib`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Ignore the system .NET framework directory when searching for assemblies.|
|[`/clr:pure`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produce an IL-only output file (no native executable code).|
|[`/clr:safe`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Produce an IL-only verifiable output file.|
|[`/EHa`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|Enable C++ exception handling (with SEH exceptions).|
|[`/EHc`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|`extern "C"` defaults to `nothrow`.|
|[`/EHr`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|Always generate `noexcept` runtime termination checks.|
|[`/EHs`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)|Enable C++ exception handling (no SEH exceptions).|
|[`/fp:contract`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|Consider floating-point contractions when generating code.|
|[`/fp:except[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|Consider floating-point exceptions when generating code.|
|[`/fp:fast`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|"fast" floating-point model; results are less predictable.|
|[`/fp:precise`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|"precise" floating-point model; results are predictable.|
|[`/fp:strict`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170)|"strict" floating-point model (implies `/fp:except`).|
|[`/fpcvt:BC`](https://learn.microsoft.com/en-us/cpp/build/reference/fpcvt?view=msvc-170)|Backward-compatible floating-point to unsigned integer conversions.|
|[`/fpcvt:IA`](https://learn.microsoft.com/en-us/cpp/build/reference/fpcvt?view=msvc-170)|Intel native floating-point to unsigned integer conversion behavior.|
|[`/fsanitize`](https://learn.microsoft.com/en-us/cpp/build/reference/fsanitize?view=msvc-170)|Enables compilation of sanitizer instrumentation such as AddressSanitizer.|
|[`/fsanitize-coverage`](https://learn.microsoft.com/en-us/cpp/build/reference/fsanitize-coverage?view=msvc-170)|Enables compilation of code coverage instrumentation for libraries such as LibFuzzer.|
|[`/GA`](https://learn.microsoft.com/en-us/cpp/build/reference/ga-optimize-for-windows-application?view=msvc-170)|Optimizes for Windows applications.|
|[`/Gd`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__cdecl`** calling convention. (x86 only)|
|[`/Ge`](https://learn.microsoft.com/en-us/cpp/build/reference/ge-enable-stack-probes?view=msvc-170)|Deprecated. Activates stack probes.|
|[`/GF`](https://learn.microsoft.com/en-us/cpp/build/reference/gf-eliminate-duplicate-strings?view=msvc-170)|Enables string pooling.|
|[`/Gh`](https://learn.microsoft.com/en-us/cpp/build/reference/gh-enable-penter-hook-function?view=msvc-170)|Calls hook function `_penter`.|
|[`/GH`](https://learn.microsoft.com/en-us/cpp/build/reference/gh-enable-pexit-hook-function?view=msvc-170)|Calls hook function `_pexit`.|
|[`/GL[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gl-whole-program-optimization?view=msvc-170)|Enables whole program optimization.|
|[`/Gm[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gm-enable-minimal-rebuild?view=msvc-170)|Deprecated. Enables minimal rebuild.|
|[`/Gr`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__fastcall`** calling convention. (x86 only)|
|[`/GR[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gr-enable-run-time-type-information?view=msvc-170)|Enables run-time type information (RTTI).|
|[`/GS[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check?view=msvc-170)|Checks buffer security.|
|[`/Gs[n]`](https://learn.microsoft.com/en-us/cpp/build/reference/gs-control-stack-checking-calls?view=msvc-170)|Controls stack probes.|
|[`/GT`](https://learn.microsoft.com/en-us/cpp/build/reference/gt-support-fiber-safe-thread-local-storage?view=msvc-170)|Supports fiber safety for data allocated by using static thread-local storage.|
|`/Gu[-]`|Ensure distinct functions have distinct addresses.|
|[`/guard:cf[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/guard-enable-control-flow-guard?view=msvc-170)|Adds control flow guard security checks.|
|[`/guard:ehcont[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/guard-enable-eh-continuation-metadata?view=msvc-170)|Enables EH continuation metadata.|
|[`/Gv`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__vectorcall`** calling convention. (x86 and x64 only)|
|[`/Gw[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gw-optimize-global-data?view=msvc-170)|Enables whole-program global data optimization.|
|[`/GX[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gx-enable-exception-handling?view=msvc-170)|Deprecated. Enables synchronous exception handling. Use [`/EH`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170) instead.|
|[`/Gy[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/gy-enable-function-level-linking?view=msvc-170)|Enables function-level linking.|
|[`/Gz`](https://learn.microsoft.com/en-us/cpp/build/reference/gd-gr-gv-gz-calling-convention?view=msvc-170)|Uses the **`__stdcall`** calling convention. (x86 only)|
|[`/GZ`](https://learn.microsoft.com/en-us/cpp/build/reference/gz-enable-stack-frame-run-time-error-checking?view=msvc-170)|Deprecated. Enables fast checks. (Same as [`/RTC1`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170))|
|[`/homeparams`](https://learn.microsoft.com/en-us/cpp/build/reference/homeparams-copy-register-parameters-to-stack?view=msvc-170)|Forces parameters passed in registers to be written to their locations on the stack upon function entry. This compiler option is only for the x64 compilers (native and cross compile).|
|[`/hotpatch`](https://learn.microsoft.com/en-us/cpp/build/reference/hotpatch-create-hotpatchable-image?view=msvc-170)|Creates a hotpatchable image.|
|[`/jumptablerdata`](https://learn.microsoft.com/en-us/cpp/build/reference/jump-table-rdata?view=msvc-170)|Put switch case statement jump tables in the `.rdata` section.|
|[`/Qfast_transcendentals`](https://learn.microsoft.com/en-us/cpp/build/reference/qfast-transcendentals-force-fast-transcendentals?view=msvc-170)|Generates fast transcendentals.|
|[`/QIfist`](https://learn.microsoft.com/en-us/cpp/build/reference/qifist-suppress-ftol?view=msvc-170)|Deprecated. Suppresses the call of the helper function `_ftol` when a conversion from a floating-point type to an integral type is required. (x86 only)|
|[`/Qimprecise_fwaits`](https://learn.microsoft.com/en-us/cpp/build/reference/qimprecise-fwaits-remove-fwaits-inside-try-blocks?view=msvc-170)|Removes `fwait` commands inside **`try`** blocks.|
|[`/QIntel-jcc-erratum`](https://learn.microsoft.com/en-us/cpp/build/reference/qintel-jcc-erratum?view=msvc-170)|Mitigates the performance impact of the Intel JCC erratum microcode update.|
|[`/Qpar`](https://learn.microsoft.com/en-us/cpp/build/reference/qpar-auto-parallelizer?view=msvc-170)|Enables automatic parallelization of loops.|
|[`/Qpar-report:n`](https://learn.microsoft.com/en-us/cpp/build/reference/qpar-report-auto-parallelizer-reporting-level?view=msvc-170)|Enables reporting levels for automatic parallelization.|
|[`/Qsafe_fp_loads`](https://learn.microsoft.com/en-us/cpp/build/reference/qsafe-fp-loads?view=msvc-170)|Uses integer move instructions for floating-point values and disables certain floating point load optimizations.|
|[`/Qspectre[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/qspectre?view=msvc-170)|Enable mitigations for CVE 2017-5753, for a class of Spectre attacks.|
|[`/Qspectre-load`](https://learn.microsoft.com/en-us/cpp/build/reference/qspectre-load?view=msvc-170)|Generate serializing instructions for every load instruction.|
|[`/Qspectre-load-cf`](https://learn.microsoft.com/en-us/cpp/build/reference/qspectre-load-cf?view=msvc-170)|Generate serializing instructions for every control flow instruction that loads memory.|
|[`/Qvec-report:n`](https://learn.microsoft.com/en-us/cpp/build/reference/qvec-report-auto-vectorizer-reporting-level?view=msvc-170)|Enables reporting levels for automatic vectorization.|
|[`/RTC1`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Enable fast runtime checks (equivalent to `/RTCsu`).|
|[`/RTCc`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Convert to smaller type checks at run-time.|
|[`/RTCs`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Enable stack frame runtime checks.|
|[`/RTCu`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170)|Enables uninitialized local usage checks.|
|[`/volatile:iso`](https://learn.microsoft.com/en-us/cpp/build/reference/volatile-volatile-keyword-interpretation?view=msvc-170)|Acquire/release semantics not guaranteed on volatile accesses.|
|[`/volatile:ms`](https://learn.microsoft.com/en-us/cpp/build/reference/volatile-volatile-keyword-interpretation?view=msvc-170)|Acquire/release semantics guaranteed on volatile accesses.|

###### Output files

|Option|Purpose|
|---|---|
|[`/doc`](https://learn.microsoft.com/en-us/cpp/build/reference/doc-process-documentation-comments-c-cpp?view=msvc-170)|Processes documentation comments to an XML file.|
|[`/FA`](https://learn.microsoft.com/en-us/cpp/build/reference/fa-fa-listing-file?view=msvc-170)|Configures an assembly listing file.|
|[`/Fa`](https://learn.microsoft.com/en-us/cpp/build/reference/fa-fa-listing-file?view=msvc-170)|Creates an assembly listing file.|
|[`/Fd`](https://learn.microsoft.com/en-us/cpp/build/reference/fd-program-database-file-name?view=msvc-170)|Renames program database file.|
|[`/Fe`](https://learn.microsoft.com/en-us/cpp/build/reference/fe-name-exe-file?view=msvc-170)|Renames the executable file.|
|[`/Fi`](https://learn.microsoft.com/en-us/cpp/build/reference/fi-preprocess-output-file-name?view=msvc-170)|Specifies the preprocessed output file name.|
|[`/Fm`](https://learn.microsoft.com/en-us/cpp/build/reference/fm-name-mapfile?view=msvc-170)|Creates a mapfile.|
|[`/Fo`](https://learn.microsoft.com/en-us/cpp/build/reference/fo-object-file-name?view=msvc-170)|Creates an object file.|
|[`/Fp`](https://learn.microsoft.com/en-us/cpp/build/reference/fp-name-dot-pch-file?view=msvc-170)|Specifies a precompiled header file name.|
|[`/FR`, `/Fr`](https://learn.microsoft.com/en-us/cpp/build/reference/fr-fr-create-dot-sbr-file?view=msvc-170)|Name generated _`.sbr`_ browser files. **`/Fr`** is deprecated.|
|`/Ft<dir>`|Location of the header files generated for `#import`.|

###### Preprocessor

|Option|Purpose|
|---|---|
|[`/AI<dir>`](https://learn.microsoft.com/en-us/cpp/build/reference/ai-specify-metadata-directories?view=msvc-170)|Specifies a directory to search to resolve file references passed to the [`#using`](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-using-directive-cpp?view=msvc-170) directive.|
|[`/C`](https://learn.microsoft.com/en-us/cpp/build/reference/c-preserve-comments-during-preprocessing?view=msvc-170)|Preserves comments during preprocessing.|
|[`/D<name>{=\|#}<text>`](https://learn.microsoft.com/en-us/cpp/build/reference/d-preprocessor-definitions?view=msvc-170)|Defines constants and macros.|
|[`/E`](https://learn.microsoft.com/en-us/cpp/build/reference/e-preprocess-to-stdout?view=msvc-170)|Copies preprocessor output to standard output.|
|[`/EP`](https://learn.microsoft.com/en-us/cpp/build/reference/ep-preprocess-to-stdout-without-hash-line-directives?view=msvc-170)|Copies preprocessor output to standard output.|
|[`/FI<file>`](https://learn.microsoft.com/en-us/cpp/build/reference/fi-name-forced-include-file?view=msvc-170)|Preprocesses the specified include file.|
|[`/FU<file>`](https://learn.microsoft.com/en-us/cpp/build/reference/fu-name-forced-hash-using-file?view=msvc-170)|Forces the use of a file name, as if it had been passed to the [`#using`](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-using-directive-cpp?view=msvc-170) directive.|
|[`/Fx`](https://learn.microsoft.com/en-us/cpp/build/reference/fx-merge-injected-code?view=msvc-170)|Merges injected code with the source file.|
|[`/I<dir>`](https://learn.microsoft.com/en-us/cpp/build/reference/i-additional-include-directories?view=msvc-170)|Searches a directory for include files.|
|[`/P`](https://learn.microsoft.com/en-us/cpp/build/reference/p-preprocess-to-a-file?view=msvc-170)|Writes preprocessor output to a file.|
|`/PD`|Print all macro definitions.|
|`/PH`|Generate `#pragma file_hash` when preprocessing.|
|[`/U<name>`](https://learn.microsoft.com/en-us/cpp/build/reference/u-u-undefine-symbols?view=msvc-170)|Removes a predefined macro.|
|[`/u`](https://learn.microsoft.com/en-us/cpp/build/reference/u-u-undefine-symbols?view=msvc-170)|Removes all predefined macros.|
|[`/X`](https://learn.microsoft.com/en-us/cpp/build/reference/x-ignore-standard-include-paths?view=msvc-170)|Ignores the standard include directory.|

###### Header units/modules

|Option|Purpose|
|---|---|
|[`/exportHeader`](https://learn.microsoft.com/en-us/cpp/build/reference/module-exportheader?view=msvc-170)|Create the header units files (_`.ifc`_) specified by the input arguments.|
|[`/headerUnit`](https://learn.microsoft.com/en-us/cpp/build/reference/headerunit?view=msvc-170)|Specify where to find the header unit file (_`.ifc`_) for the specified header.|
|[`/headerName`](https://learn.microsoft.com/en-us/cpp/build/reference/headername?view=msvc-170)|Build a header unit from the specified header.|
|[`/ifcOutput`](https://learn.microsoft.com/en-us/cpp/build/reference/ifc-output?view=msvc-170)|Specify the output file name or directory for built _`.ifc`_ files.|
|[`/interface`](https://learn.microsoft.com/en-us/cpp/build/reference/interface?view=msvc-170)|Treat the input file as a module interface unit.|
|[`/internalPartition`](https://learn.microsoft.com/en-us/cpp/build/reference/internal-partition?view=msvc-170)|Treat the input file as an internal partition unit.|
|[`/reference`](https://learn.microsoft.com/en-us/cpp/build/reference/module-reference?view=msvc-170)|Use named module IFC.|
|[`/scanDependencies`](https://learn.microsoft.com/en-us/cpp/build/reference/scandependencies?view=msvc-170)|List module and header unit dependencies in C++ Standard JSON form.|
|[`/sourceDependencies`](https://learn.microsoft.com/en-us/cpp/build/reference/sourcedependencies?view=msvc-170)|List all source-level dependencies.|
|[`/sourceDependencies:directives`](https://learn.microsoft.com/en-us/cpp/build/reference/sourcedependencies-directives?view=msvc-170)|List module and header unit dependencies.|
|[`/translateInclude`](https://learn.microsoft.com/en-us/cpp/build/reference/translateinclude?view=msvc-170)|Treat `#include` as `import`.|

###### Language

|Option|Purpose|
|---|---|
|[`/await`](https://learn.microsoft.com/en-us/cpp/build/reference/await-enable-coroutine-support?view=msvc-170)|Enable coroutines (resumable functions) extensions.|
|[`/await:strict`](https://learn.microsoft.com/en-us/cpp/build/reference/await-enable-coroutine-support?view=msvc-170)|Enable standard C++20 coroutine support with earlier language versions.|
|[`/constexpr:backtrace<N>`](https://learn.microsoft.com/en-us/cpp/build/reference/constexpr-control-constexpr-evaluation?view=msvc-170)|Show N `constexpr` evaluations in diagnostics (default: 10).|
|[`/constexpr:depth<N>`](https://learn.microsoft.com/en-us/cpp/build/reference/constexpr-control-constexpr-evaluation?view=msvc-170)|Recursion depth limit for `constexpr` evaluation (default: 512).|
|[`/constexpr:steps<N>`](https://learn.microsoft.com/en-us/cpp/build/reference/constexpr-control-constexpr-evaluation?view=msvc-170)|Terminate `constexpr` evaluation after N steps (default: 100000)|
|[`/openmp`](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=msvc-170)|Enables [`#pragma omp`](https://learn.microsoft.com/en-us/cpp/preprocessor/omp?view=msvc-170) in source code.|
|[`/openmp:experimental`](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=msvc-170)|Enable OpenMP 2.0 language extensions plus select OpenMP 3.0+ language extensions.|
|[`/openmp:llvm`](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support?view=msvc-170)|OpenMP language extensions using LLVM runtime.|
|[`/permissive[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance?view=msvc-170)|Set standard-conformance mode.|
|[`/std:c++14`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C++14 standard ISO/IEC 14882:2014 (default).|
|[`/std:c++17`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C++17 standard ISO/IEC 14882:2017.|
|[`/std:c++20`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C++20 standard ISO/IEC 14882:2020.|
|[`/std:c++latest`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|The latest draft C++ standard preview features.|
|[`/std:c11`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C11 standard ISO/IEC 9899:2011.|
|[`/std:c17`](https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)|C17 standard ISO/IEC 9899:2018.|
|[`/vd{0\|1\|2}`](https://learn.microsoft.com/en-us/cpp/build/reference/vd-disable-construction-displacements?view=msvc-170)|Suppresses or enables hidden `vtordisp` class members.|
|[`/vmb`](https://learn.microsoft.com/en-us/cpp/build/reference/vmb-vmg-representation-method?view=msvc-170)|Uses best base for pointers to members.|
|[`/vmg`](https://learn.microsoft.com/en-us/cpp/build/reference/vmb-vmg-representation-method?view=msvc-170)|Uses full generality for pointers to members.|
|[`/vmm`](https://learn.microsoft.com/en-us/cpp/build/reference/vmm-vms-vmv-general-purpose-representation?view=msvc-170)|Declares multiple inheritance.|
|[`/vms`](https://learn.microsoft.com/en-us/cpp/build/reference/vmm-vms-vmv-general-purpose-representation?view=msvc-170)|Declares single inheritance.|
|[`/vmv`](https://learn.microsoft.com/en-us/cpp/build/reference/vmm-vms-vmv-general-purpose-representation?view=msvc-170)|Declares virtual inheritance.|
|[`/Z7`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170)|Generates C 7.0-compatible debugging information.|
|[`/Za`](https://learn.microsoft.com/en-us/cpp/build/reference/za-ze-disable-language-extensions?view=msvc-170)|Disables some C89 language extensions in C code.|
|[`/Zc:__cplusplus[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-170)|Enable the `__cplusplus` macro to report the supported standard (off by default).|
|[`/Zc:__STDC__`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-stdc?view=msvc-170)|Enable the `__STDC__` macro to report the C standard is supported (off by default).|
|[`/Zc:alignedNew[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-alignednew?view=msvc-170)|Enable C++17 over-aligned dynamic allocation (on by default in C++17).|
|[`/Zc:auto[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-auto-deduce-variable-type?view=msvc-170)|Enforce the new Standard C++ meaning for **`auto`** (on by default).|
|[`/Zc:char8_t[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-char8-t?view=msvc-170)|Enable or disable C++20 native `u8` literal support as `const char8_t` (off by default, except under **`/std:c++20`**).|
|[`/Zc:enumTypes[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-enumtypes?view=msvc-170)|Enable Standard C++ rules for inferred `enum` base types (Off b y default, not implied by **`/permissive-`**).|
|[`/Zc:externC[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-externc?view=msvc-170)|Enforce Standard C++ rules for `extern "C"` functions (implied by **`/permissive-`**).|
|[`/Zc:externConstexpr[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-externconstexpr?view=msvc-170)|Enable external linkage for **`constexpr`** variables (off by default).|
|[`/Zc:forScope[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-forscope-force-conformance-in-for-loop-scope?view=msvc-170)|Enforce Standard C++ **`for`** scoping rules (on by default).|
|[`/Zc:gotoScope`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-gotoscope?view=msvc-170)|Enforce Standard C++ **`goto`** rules around local variable initialization (implied by **`/permissive-`**).|
|[`/Zc:hiddenFriend[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-hiddenfriend?view=msvc-170)|Enforce Standard C++ hidden friend rules (implied by **`/permissive-`**)|
|[`/Zc:implicitNoexcept[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-implicitnoexcept-implicit-exception-specifiers?view=msvc-170)|Enable implicit **`noexcept`** on required functions (on by default).|
|[`/Zc:inline[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-inline-remove-unreferenced-comdat?view=msvc-170)|Remove unreferenced functions or data if they're COMDAT or have internal linkage only (off by default).|
|[`/Zc:lambda[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-lambda?view=msvc-170)|Enable new lambda processor for conformance-mode syntactic checks in generic lambdas.|
|[`/Zc:noexceptTypes[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-noexcepttypes?view=msvc-170)|Enforce C++17 **`noexcept`** rules (on by default in C++17 or later).|
|[`/Zc:nrvo[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-nrvo?view=msvc-170)|Enable optional copy and move elisions (on by default under **`/O2`**, **`/permissive-`**, or **`/std:c++20`** or later).|
|[`/Zc:preprocessor[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-preprocessor?view=msvc-170)|Use the new conforming preprocessor (off by default, except in C11/C17).|
|[`/Zc:referenceBinding[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-referencebinding-enforce-reference-binding-rules?view=msvc-170)|A UDT temporary won't bind to a non-const lvalue reference (off by default).|
|[`/Zc:rvalueCast[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-rvaluecast-enforce-type-conversion-rules?view=msvc-170)|Enforce Standard C++ explicit type conversion rules (off by default).|
|[`/Zc:sizedDealloc[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-sizeddealloc-enable-global-sized-dealloc-functions?view=msvc-170)|Enable C++14 global sized deallocation functions (on by default).|
|[`/Zc:strictStrings[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-strictstrings-disable-string-literal-type-conversion?view=msvc-170)|Disable string-literal to `char*` or `wchar_t*` conversion (off by default).|
|[`/Zc:templateScope[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-templatescope?view=msvc-170)|Enforce Standard C++ template parameter shadowing rules (off by default).|
|[`/Zc:ternary[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-ternary?view=msvc-170)|Enforce conditional operator rules on operand types (off by default).|
|[`/Zc:threadSafeInit[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-threadsafeinit-thread-safe-local-static-initialization?view=msvc-170)|Enable thread-safe local static initialization (on by default).|
|[`/Zc:throwingNew[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-throwingnew-assume-operator-new-throws?view=msvc-170)|Assume **`operator new`** throws on failure (off by default).|
|[`/Zc:tlsGuards[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-tlsguards?view=msvc-170)|Generate runtime checks for TLS variable initialization (on by default).|
|[`/Zc:trigraphs`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-trigraphs-trigraphs-substitution?view=msvc-170)|Enable trigraphs (obsolete, off by default).|
|[`/Zc:twoPhase[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-twophase?view=msvc-170)|Use nonconforming template parsing behavior (conforming by default).|
|[`/Zc:wchar_t[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-wchar-t-wchar-t-is-native-type?view=msvc-170)|**`wchar_t`** is a native type, not a typedef (on by default).|
|[`/Zc:zeroSizeArrayNew[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-zerosizearraynew?view=msvc-170)|Call member `new`/`delete` for 0-size arrays of objects (on by default).|
|[`/Ze`](https://learn.microsoft.com/en-us/cpp/build/reference/za-ze-disable-language-extensions?view=msvc-170)|Deprecated. Enables C89 language extensions.|
|[`/Zf`](https://learn.microsoft.com/en-us/cpp/build/reference/zf?view=msvc-170)|Improves PDB generation time in parallel builds.|
|[`/ZH:[MD5\|SHA1\|SHA_256]`](https://learn.microsoft.com/en-us/cpp/build/reference/zh?view=msvc-170)|Specifies MD5, SHA-1, or SHA-256 for checksums in debug info.|
|[`/ZI`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170)|Includes debug information in a program database compatible with Edit and Continue. (x86 only)|
|[`/Zi`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170)|Generates complete debugging information.|
|[`/Zl`](https://learn.microsoft.com/en-us/cpp/build/reference/zl-omit-default-library-name?view=msvc-170)|Removes the default library name from the _`.obj`_ file.|
|[`/Zo[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/zo-enhance-optimized-debugging?view=msvc-170)|Generate richer debugging information for optimized code.|
|[`/Zp[n]`](https://learn.microsoft.com/en-us/cpp/build/reference/zp-struct-member-alignment?view=msvc-170)|Packs structure members.|
|[`/Zs`](https://learn.microsoft.com/en-us/cpp/build/reference/zs-syntax-check-only?view=msvc-170)|Checks syntax only.|
|[`/ZW`](https://learn.microsoft.com/en-us/cpp/build/reference/zw-windows-runtime-compilation?view=msvc-170)|Produces an output file to run on the Windows Runtime.|

###### Linking

|Option|Purpose|
|---|---|
|[`/F`](https://learn.microsoft.com/en-us/cpp/build/reference/f-set-stack-size?view=msvc-170)|Sets stack size.|
|[`/LD`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Creates a dynamic-link library.|
|[`/LDd`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Creates a debug dynamic-link library.|
|[`/link`](https://learn.microsoft.com/en-us/cpp/build/reference/link-pass-options-to-linker?view=msvc-170)|Passes the specified option to LINK.|
|[`/LN`](https://learn.microsoft.com/en-us/cpp/build/reference/ln-create-msil-module?view=msvc-170)|Creates an MSIL `.netmodule`.|
|[`/MD`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a multithreaded DLL, by using _MSVCRT.lib_.|
|[`/MDd`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a debug multithreaded DLL, by using _MSVCRTD.lib_.|
|[`/MT`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a multithreaded executable file, by using _LIBCMT.lib_.|
|[`/MTd`](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170)|Compiles to create a debug multithreaded executable file, by using _LIBCMTD.lib_.|

###### Miscellaneous

|Option|Purpose|
|---|---|
|[`/?`](https://learn.microsoft.com/en-us/cpp/build/reference/help-compiler-command-line-help?view=msvc-170)|Lists the compiler options.|
|[`@`](https://learn.microsoft.com/en-us/cpp/build/reference/at-specify-a-compiler-response-file?view=msvc-170)|Specifies a response file.|
|[`/analyze`](https://learn.microsoft.com/en-us/cpp/build/reference/analyze-code-analysis?view=msvc-170)|Enables code analysis.|
|[`/bigobj`](https://learn.microsoft.com/en-us/cpp/build/reference/bigobj-increase-number-of-sections-in-dot-obj-file?view=msvc-170)|Increases the number of addressable sections in an .obj file.|
|[`/c`](https://learn.microsoft.com/en-us/cpp/build/reference/c-compile-without-linking?view=msvc-170)|Compiles without linking.|
|[`/cgthreads`](https://learn.microsoft.com/en-us/cpp/build/reference/cgthreads-code-generation-threads?view=msvc-170)|Specifies number of _cl.exe_ threads to use for optimization and code generation.|
|[`/errorReport`](https://learn.microsoft.com/en-us/cpp/build/reference/errorreport-report-internal-compiler-errors?view=msvc-170)|Deprecated. [Windows Error Reporting (WER)](https://learn.microsoft.com/en-us/windows/win32/wer/windows-error-reporting) settings control error reporting.|
|[`/execution-charset`](https://learn.microsoft.com/en-us/cpp/build/reference/execution-charset-set-execution-character-set?view=msvc-170)|Set execution character set.|
|`/fastfail`|Enable fast-fail mode.|
|[`/FC`](https://learn.microsoft.com/en-us/cpp/build/reference/fc-full-path-of-source-code-file-in-diagnostics?view=msvc-170)|Displays the full path of source code files passed to _cl.exe_ in diagnostic text.|
|[`/FS`](https://learn.microsoft.com/en-us/cpp/build/reference/fs-force-synchronous-pdb-writes?view=msvc-170)|Forces writes to the PDB file to be serialized through _MSPDBSRV.EXE_.|
|[`/H`](https://learn.microsoft.com/en-us/cpp/build/reference/h-restrict-length-of-external-names?view=msvc-170)|Deprecated. Restricts the length of external (public) names.|
|[`/HELP`](https://learn.microsoft.com/en-us/cpp/build/reference/help-compiler-command-line-help?view=msvc-170)|Lists the compiler options.|
|[`/J`](https://learn.microsoft.com/en-us/cpp/build/reference/j-default-char-type-is-unsigned?view=msvc-170)|Changes the default **`char`** type.|
|[`/JMC`](https://learn.microsoft.com/en-us/cpp/build/reference/jmc?view=msvc-170)|Supports native C++ Just My Code debugging.|
|[`/kernel`](https://learn.microsoft.com/en-us/cpp/build/reference/kernel-create-kernel-mode-binary?view=msvc-170)|The compiler and linker create a binary that can be executed in the Windows kernel.|
|[`/MP`](https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes?view=msvc-170)|Builds multiple source files concurrently.|
|[`/nologo`](https://learn.microsoft.com/en-us/cpp/build/reference/nologo-suppress-startup-banner-c-cpp?view=msvc-170)|Suppresses display of sign-on banner.|
|`/presetPadding`|Zero initialize padding for stack based class types.|
|[`/showIncludes`](https://learn.microsoft.com/en-us/cpp/build/reference/showincludes-list-include-files?view=msvc-170)|Displays a list of all include files during compilation.|
|[`/source-charset`](https://learn.microsoft.com/en-us/cpp/build/reference/source-charset-set-source-character-set?view=msvc-170)|Set source character set.|
|[`/Tc`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies a C source file.|
|[`/TC`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies all source files are C.|
|[`/Tp`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies a C++ source file.|
|[`/TP`](https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170)|Specifies all source files are C++.|
|[`/utf-8`](https://learn.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=msvc-170)|Set source and execution character sets to UTF-8.|
|[`/V`](https://learn.microsoft.com/en-us/cpp/build/reference/v-version-number?view=msvc-170)|Deprecated. Sets the version string.|
|[`/validate-charset`](https://learn.microsoft.com/en-us/cpp/build/reference/validate-charset-validate-for-compatible-characters?view=msvc-170)|Validate UTF-8 files for only compatible characters.|
|`/volatileMetadata`|Generate metadata on volatile memory accesses.|
|[`/Yc`](https://learn.microsoft.com/en-us/cpp/build/reference/yc-create-precompiled-header-file?view=msvc-170)|Create _`.PCH`_ file.|
|[`/Yd`](https://learn.microsoft.com/en-us/cpp/build/reference/yd-place-debug-information-in-object-file?view=msvc-170)|Deprecated. Places complete debugging information in all object files. Use [`/Zi`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170) instead.|
|[`/Yl`](https://learn.microsoft.com/en-us/cpp/build/reference/yl-inject-pch-reference-for-debug-library?view=msvc-170)|Injects a PCH reference when creating a debug library.|
|[`/Yu`](https://learn.microsoft.com/en-us/cpp/build/reference/yu-use-precompiled-header-file?view=msvc-170)|Uses a precompiled header file during build.|
|[`/Y-`](https://learn.microsoft.com/en-us/cpp/build/reference/y-ignore-precompiled-header-options?view=msvc-170)|Ignores all other precompiled-header compiler options in the current build.|
|[`/Zm`](https://learn.microsoft.com/en-us/cpp/build/reference/zm-specify-precompiled-header-memory-allocation-limit?view=msvc-170)|Specifies the precompiled header memory allocation limit.|

###### Diagnostics

|Option|Purpose|
|---|---|
|[`/diagnostics:caret[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options?view=msvc-170)|Diagnostics format: prints column and the indicated line of source.|
|[`/diagnostics:classic`](https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options?view=msvc-170)|Use legacy diagnostics format.|
|[`/diagnostics`](https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options?view=msvc-170)|Diagnostics format: prints column information.|
|[`/external:anglebrackets`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Treat all headers included via `<>` as external.|
|[`/external:env:<var>`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Specify an environment variable with locations of external headers.|
|[`/external:I <path>`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Specify location of external headers.|
|[`/external:templates[-]`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Evaluate warning level across template instantiation chain.|
|[`/external:W<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170)|Set warning level for external headers.|
|[`/options:strict`](https://learn.microsoft.com/en-us/cpp/build/reference/options-strict?view=msvc-170)|Unrecognized compiler options are errors.|
|[`/sdl`](https://learn.microsoft.com/en-us/cpp/build/reference/sdl-enable-additional-security-checks?view=msvc-170)|Enable more security features and warnings.|
|[`/w`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Disable all warnings.|
|[`/W0`, `/W1`, `/W2`, `/W3`, `/W4`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Set output warning level.|
|[`/w1<n>`, `/w2<n>`, `/w3<n>`, `/w4<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Set warning level for the specified warning.|
|[`/Wall`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Enable all warnings, including warnings that are disabled by default.|
|[`/wd<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Disable the specified warning.|
|[`/we<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Treat the specified warning as an error.|
|[`/WL`](https://learn.microsoft.com/en-us/cpp/build/reference/wl-enable-one-line-diagnostics?view=msvc-170)|Enable one-line diagnostics for error and warning messages when compiling C++ source code from the command line.|
|[`/wo<n>`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Display the specified warning only once.|
|[`/Wv:xx[.yy[.zzzzz]]`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Disable warnings introduced after the specified version of the compiler.|
|[`/WX`](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170)|Treat warnings as errors.|

###### Experimental options

Experimental options may only be supported by certain versions of the compiler. They may also behave differently in different compiler versions. Often the best, or only, documentation for experimental options is in the [Microsoft C++ Team Blog](https://devblogs.microsoft.com/cppblog/).

|Option|Purpose|
|---|---|
|[`/experimental:log`](https://learn.microsoft.com/en-us/cpp/build/reference/experimental-log?view=msvc-170)|Enables experimental structured SARIF output.|
|[`/experimental:module`](https://learn.microsoft.com/en-us/cpp/build/reference/experimental-module?view=msvc-170)|Enables experimental module support.|

###### Deprecated and removed compiler options

|Option|Purpose|
|---|---|
|[`/clr:noAssembly`](https://learn.microsoft.com/en-us/cpp/build/reference/clr-common-language-runtime-compilation?view=msvc-170)|Deprecated. Use [`/LN` (Create MSIL Module)](https://learn.microsoft.com/en-us/cpp/build/reference/ln-create-msil-module?view=msvc-170) instead.|
|[`/errorReport`](https://learn.microsoft.com/en-us/cpp/build/reference/errorreport-report-internal-compiler-errors?view=msvc-170)|Deprecated. Error reporting is controlled by [Windows Error Reporting (WER)](https://learn.microsoft.com/en-us/windows/win32/wer/windows-error-reporting) settings.|
|[`/experimental:preprocessor`](https://learn.microsoft.com/en-us/cpp/build/reference/experimental-preprocessor?view=msvc-170)|Deprecated. Enables experimental conforming preprocessor support. Use [`/Zc:preprocessor`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-preprocessor?view=msvc-170)|
|[`/Fr`](https://learn.microsoft.com/en-us/cpp/build/reference/fr-fr-create-dot-sbr-file?view=msvc-170)|Deprecated. Creates a browse information file without local variables.|
|[`/Ge`](https://learn.microsoft.com/en-us/cpp/build/reference/ge-enable-stack-probes?view=msvc-170)|Deprecated. Activates stack probes. On by default.|
|[`/Gm`](https://learn.microsoft.com/en-us/cpp/build/reference/gm-enable-minimal-rebuild?view=msvc-170)|Deprecated. Enables minimal rebuild.|
|[`/GX`](https://learn.microsoft.com/en-us/cpp/build/reference/gx-enable-exception-handling?view=msvc-170)|Deprecated. Enables synchronous exception handling. Use [`/EH`](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170) instead.|
|[`/GZ`](https://learn.microsoft.com/en-us/cpp/build/reference/gz-enable-stack-frame-run-time-error-checking?view=msvc-170)|Deprecated. Enables fast checks. Use [`/RTC1`](https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks?view=msvc-170) instead.|
|[`/H`](https://learn.microsoft.com/en-us/cpp/build/reference/h-restrict-length-of-external-names?view=msvc-170)|Deprecated. Restricts the length of external (public) names.|
|[`/Og`](https://learn.microsoft.com/en-us/cpp/build/reference/og-global-optimizations?view=msvc-170)|Deprecated. Uses global optimizations.|
|[`/QIfist`](https://learn.microsoft.com/en-us/cpp/build/reference/qifist-suppress-ftol?view=msvc-170)|Deprecated. Once used to specify how to convert from a floating-point type to an integral type.|
|[`/V`](https://learn.microsoft.com/en-us/cpp/build/reference/v-version-number?view=msvc-170)|Deprecated. Sets the _`.obj`_ file version string.|
|[`/Wp64`](https://learn.microsoft.com/en-us/cpp/build/reference/wp64-detect-64-bit-portability-issues?view=msvc-170)|Obsolete. Detects 64-bit portability problems.|
|[`/Yd`](https://learn.microsoft.com/en-us/cpp/build/reference/yd-place-debug-information-in-object-file?view=msvc-170)|Deprecated. Places complete debugging information in all object files. Use [`/Zi`](https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170) instead.|
|[`/Zc:forScope-`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-forscope-force-conformance-in-for-loop-scope?view=msvc-170)|Deprecated. Disables conformance in for loop scope.|
|[`/Ze`](https://learn.microsoft.com/en-us/cpp/build/reference/za-ze-disable-language-extensions?view=msvc-170)|Deprecated. Enables language extensions.|
|[`/Zg`](https://learn.microsoft.com/en-us/cpp/build/reference/zg-generate-function-prototypes?view=msvc-170)|Removed in Visual Studio 2015. Generates function prototypes.|




---



## [`Link`](https://learn.microsoft.com/zh-cn/cpp/build/reference/linking?view=msvc-170)  
### 使用链接器 (link.exe) 可将已编译的对象文件和库链接到应用和 DLL 中。