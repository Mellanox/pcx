Persistent Collectives X- A collective communication library for high performance, low cost persistent collectives over RDMA devices.

This library is aimed at providing a minimal latency persistent collectives, while minimizing CPU/GPU utilization to near-zero.
This is done by taking advantage of the persistent attributes of the communication, while using several different technologies- 
Including: RDMA, Core-Direct, NIC calculation capabilities, peer-direct and network device memory (when available),
in combination with (almost) complete sofware bypass on data-path.

PCX provides a (relatively) easy, c++ based mid-level interface for implementation of high performance collectives using simple commands.

PCX is currently in a PoC stage, targeted at accelerating Deep learning distributed training.

Requirements:
Using PCX requires special firmware modifications (Needed to ignore wqe index check, so we can recycle wqes)
As a temporary workaround, a shell script for disabling wqe checks in connect-X5 is provided.

use:
sudo ./disable_wqe_checks.sh
At your own risk, to enable expirimenting with pcx (May need to modify the device beforehand)

use:
sudo ./enable_wqe_checks.sh
To hopefully undo any damage the previous script might have done.

Examples:
Examples for implementation of collectives over PCX can be seen here:

[Recursive Doubling](https://github.com/yanivbl6/gloo/blob/yaniv_branch/gloo/pcx_allreduce_king.h)

[Ring](https://github.com/yanivbl6/gloo/blob/yaniv_branch/gloo/pcx_allreduce_ring.h)

