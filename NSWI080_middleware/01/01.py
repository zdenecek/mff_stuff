import grpc
from activity_pb2 import StartHereRequest  
from activity_pb2_grpc import LabActivityStub  


def main():
    channel = grpc.insecure_channel('lab.d3s.mff.cuni.cz:6001')
    
    stub = LabActivityStub(channel)
    
    request = StartHereRequest(sis_login="78002598")
    
    try:
        response = stub.StartHere(request)
        print("Server instructions:", response.instructions)
    except grpc.RpcError as e:
        print(f"RPC failed: {e}")
    finally:
        channel.close()

if __name__ == "__main__":
    main()