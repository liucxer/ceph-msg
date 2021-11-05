set(msg_srcs
  DispatchQueue.cc
  Message.cc
  Messenger.cc
  QueueStrategy.cc
  msg_types.cc
  simple/Accepter.cc
  simple/Pipe.cc
  simple/PipeConnection.cc
  simple/SimpleMessenger.cc)

if(HAVE_XIO)
  list(APPEND msg_srcs
    xio/XioConnection.cc
    xio/XioMsg.cc
    xio/XioPool.cc
    xio/XioMessenger.cc
    xio/XioPortal.cc)
endif(HAVE_XIO)

list(APPEND msg_srcs
  async/AsyncConnection.cc
  async/AsyncMessenger.cc
  async/Protocol.cc
  async/ProtocolV1.cc
  async/ProtocolV2.cc
  async/Event.cc
  async/EventSelect.cc
  async/PosixStack.cc
  async/Stack.cc
  async/crypto_onwire.cc
  async/net_handler.cc)

if(LINUX)
  list(APPEND msg_srcs
    async/EventEpoll.cc)
elseif(FREEBSD OR APPLE)
  list(APPEND msg_srcs
    async/EventKqueue.cc)
endif(LINUX)

add_library(common-msg-objs OBJECT ${msg_srcs})
