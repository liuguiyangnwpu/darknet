# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

from rpc import darknet_pb2 as darknet__pb2
# import darknet_pb2 as darknet__pb2


class DetectRpcStub(object):

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.Detect = channel.unary_unary(
        '/darknet.DetectRpc/Detect',
        request_serializer=darknet__pb2.DetectRequest.SerializeToString,
        response_deserializer=darknet__pb2.DetectResponse.FromString,
        )


class DetectRpcServicer(object):

  def Detect(self, request, context):
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_DetectRpcServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'Detect': grpc.unary_unary_rpc_method_handler(
          servicer.Detect,
          request_deserializer=darknet__pb2.DetectRequest.FromString,
          response_serializer=darknet__pb2.DetectResponse.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'darknet.DetectRpc', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))