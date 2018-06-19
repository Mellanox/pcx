#pragma once

#include "dv_mgr.h"
#include "verbs_ctx.h"

#include <functional>
#include <queue>
#include <vector>

enum cd_statuses { PCOLL_SUCCESS = 0, PCOLL_ERROR = 1 };

typedef std::function<void()> LambdaInstruction;
typedef std::queue<LambdaInstruction> InsQueue;

class PcxQp;
class ManagementQp;

typedef std::vector<PcxQp *> GraphQps;
typedef GraphQps::iterator GraphQpsIt;

class CommGraph {
public:
  CommGraph(VerbCtx *vctx);
  ~CommGraph();

  void enqueue(LambdaInstruction &ins);

  void regQp(PcxQp *qp);
  void wait(PcxQp *slave_qp);
  void wait_send(PcxQp *slave_qp);

  void db();

  void finish();

  ManagementQp *mqp;

  VerbCtx *ctx;
  InsQueue iq;
  GraphQps qps;
  uint16_t qp_cnt;
};

class PcxQp {
public:
  PcxQp(CommGraph *cgraph);
  virtual ~PcxQp() = 0;
  virtual void init() = 0;
  void fin();
  void sendCredit();
  void write(NetMem *local, NetMem *remote);
  void writeCmpl(NetMem *local, NetMem *remote);
  void reduce_write(NetMem *local, NetMem *remote, uint16_t num_vectors,
                    uint8_t op, uint8_t type);

  void poll();
  void db(uint32_t k);
  void print();
  void db();

  int wqe_count;
  int cqe_count;
  int scqe_count;
  int recv_enables;

  uint16_t id;
  qp_ctx *qp;

protected:
  CommGraph *graph;
  struct ibv_qp *ibqp;
  struct ibv_cq *ibcq;
  struct ibv_cq *ibscq;
  PcxQp *pair;
  bool initiated;
  bool has_scq;
  VerbCtx *ctx;
};

class ManagementQp : public PcxQp {
public:
  void cd_send_enable(PcxQp *slave_qp);
  void cd_recv_enable(PcxQp *slave_qp);
  void cd_wait(PcxQp *slave_qp);
  void cd_wait_send(PcxQp *slave_qp);

  ManagementQp(CommGraph *cgraph);
  ~ManagementQp();
  void init();

  LambdaInstruction stack;
  uint16_t last_qp;
  bool has_stack;
};

class LoopbackQp : public PcxQp {
public:
  LoopbackQp(CommGraph *cgraph);
  ~LoopbackQp();
  void init();
};

/*
typedef enum PCX_P2P_RESULT{
  PCX_P2P_SUCCESS,
  PCX_P2P_FAILURE,
}*/

typedef struct rd_peer_info {
  uintptr_t buf;
  union {
    uint32_t rkey;
    uint32_t lkey;
  };
  peer_addr_t addr;
} rd_peer_info_t;

typedef int (*p2p_exchange_func)(void *, volatile void *, volatile void *,
                                 size_t, uint32_t, uint32_t);
typedef std::function<void(volatile void *, volatile void *, size_t)>
    LambdaExchange;

class RcQp : public PcxQp {
public:
  RcQp(CommGraph *cgraph, PipeMem *incoming_)
      : PcxQp(cgraph), incoming(incoming_){};
  virtual ~RcQp();

  void write(NetMem *local, size_t pos = 0);
  void writeCmpl(NetMem *local, size_t pos = 0);
  void reduce_write(NetMem *local, size_t pos, uint16_t num_vectors, uint8_t op,
                    uint8_t type);
  void reduce_write_cmpl(NetMem *local, size_t pos, uint16_t num_vectors,
                         uint8_t op, uint8_t type);

  void setPair(PcxQp *pair_) { this->pair = pair_; };

protected:
  PipeMem *remote;
  PipeMem *incoming;
};

class DoublingQp : public PcxQp {
public:
  DoublingQp(CommGraph *cgraph, p2p_exchange_func func, void *comm,
             uint32_t peer, uint32_t tag, NetMem *incomingBuffer);
  ~DoublingQp();
  void init();
  void write(NetMem *local);
  void writeCmpl(NetMem *local);

  RemoteMem *remote;
  NetMem *incoming;
  LambdaExchange exchange;
};

class RingQp : public RcQp {
public:
  RingQp(CommGraph *cgraph, p2p_exchange_func func, void *comm, uint32_t peer,
         uint32_t tag, PipeMem *incoming);

  void init();
  ~RingQp();
  LambdaExchange exchange;
};

class RingPair {
public:
  RingPair(CommGraph *cgraph, p2p_exchange_func func, void *comm,
           uint32_t myRank, uint32_t commSize, uint32_t tag1, uint32_t tag2,
           PipeMem *incoming);
  ~RingPair();

  RingQp *right;
  RingQp *left;
};

struct ibv_qp *create_management_qp(struct ibv_cq *cq, VerbCtx *verb_ctx,
                                    uint16_t send_wq_size);

struct ibv_qp *rc_qp_create(struct ibv_cq *cq, VerbCtx *verb_ctx,
                            uint16_t send_wq_size, uint16_t recv_rq_size,
                            struct ibv_cq *s_cq = NULL, int slaveRecv = 1,
                            int slaveSend = 1);

struct ibv_cq *cd_create_cq(VerbCtx *verb_ctx, int cqe, void *cq_context = NULL,
                            struct ibv_comp_channel *channel = NULL,
                            int comp_vector = 0);
