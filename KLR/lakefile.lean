import Lake
open Lake DSL

package Util

@[default_target]
lean_lib Util

require plausible from git
  "https://github.com/leanprover-community/plausible" @ "v4.19.0"

require TensorLib from git
  "https://github.com/leanprover/TensorLib.git" @ "v0.0.10"
