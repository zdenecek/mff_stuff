import grpc
import asyncio
from guess_pb2 import Guess
from guess_pb2_grpc import GuessTheNumberStub

async def main():
    async with grpc.aio.insecure_channel('lab.d3s.mff.cuni.cz:6001') as channel:
        stub = GuessTheNumberStub(channel)
        metadata = [('sis-login', '78002598')]
        call = stub.Play(metadata=metadata)

        low = 1
        high = 1_000_000
        curr = None

        while low <= high:
            curr = (low + high) // 2
            print(f"trying {curr}")
            await call.write(Guess(guess=curr))
            response = await call.read()
            
            if response.WhichOneof('result') == 'reason':
                if response.reason == 0:  
                    print("Response: TOO_LOW")
                    low = curr + 1
                elif response.reason == 1:  
                    print("Response: TOO_HIGH")
                    high = curr - 1
            else:
                print("Game won!!")
                print("Instructions:", response.instructions)
                break

        await call.done_writing()

if __name__ == '__main__':
    asyncio.run(main())
