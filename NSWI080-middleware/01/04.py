import grpc
from noise_pb2 import MonitorMeasurementsRequest, TemperatureEstimate
from noise_pb2_grpc import NoisySensorStub

def main():
    channel = grpc.insecure_channel('lab.d3s.mff.cuni.cz:6001')
    stub = NoisySensorStub(channel)
    metadata = [('sis-login', '78002598')]
    
    try:
        temps = []
        
        for measurement in stub.MonitorMeasurements(MonitorMeasurementsRequest(), metadata=metadata):
            temp = measurement.temperature
            print(f"Got temperature: {temp}")
            temps.append(temp)
            
            if len(temps) >= 30:
                break
        
        avg_temp = sum(temps) / len(temps)
        print(f"\nGot {len(temps)} measurements")
        print(f"Average temperature looks like: {avg_temp}")
        
        print("\nSubmitting estimate...")
        response = stub.SubmitTemperatureEstimate(TemperatureEstimate(temperature=avg_temp), metadata=metadata)
        
        if response.success:
            print("nYES! Got it!")
            print("Next task:", response.instructions)
        else:
            print("\nDamn, wrong guess :(")
            
    except grpc.RpcError as e:
        print(f"RPC failed: {e}")
    finally:
        channel.close()

if __name__ == "__main__":
    main()