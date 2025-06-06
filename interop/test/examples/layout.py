from apis import *

def tensor_exp_kernel_1(in_tensor, out_tensor):
  """NKI kernel to compute elementwise exponential of an input tensor

  Args:
      in_tensor: an input tensor of shape [256,512]
      out_tensor: an output tensor of shape [256,512]
  """
  i_f = nl.arange(512)[None, :]

  for k in nl.affine_range(2):
    # Generate tensor indices for the input/output tensors
    i_p = k * nl.tile_size.pmax + nl.arange(nl.tile_size.pmax)[:, None]

    # Load input data from HBM to on-chip memory
    in_tile = nl.load(in_tensor[i_p, i_f])

    # perform the computation
    out_tile = nl.exp(in_tile)

    # store the results back to HBM
    nl.store(out_tensor[i_p, i_f], value=out_tile)


def tensor_exp_kernel_2(in_tensor, out_tensor):
  """NKI kernel to compute elementwise exponential of an input tensor

  Args:
      in_tensor: an input tensor of shape [128,512]
      out_tensor: an output tensor of shape [128,512]
  """
  # Generate indices for the input/output tensors
  i_p = nl.arange(128)[:, None]
  i_f = nl.arange(512)[None, :]

  # Load input data from HBM to on-chip memory
  in_tile = nl.load(in_tensor[i_p, i_f])

  # perform the computation:
  out_tile = nl.exp(in_tile)

  # store the results back to HBM
  nl.store(out_tensor[i_p, i_f], value=out_tile)
