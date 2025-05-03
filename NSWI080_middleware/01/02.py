import grpc
from grocery_pb2 import CreateShoppingCartRequest, AddProductToCartRequest, ViewProductsRequest, CheckoutRequest
from grocery_pb2_grpc import GroceryShoppingServiceStub

def main():
    c = grpc.insecure_channel('lab.d3s.mff.cuni.cz:6001')
    meta = [('sis-login', '78002598')]
    s = GroceryShoppingServiceStub(c)
    
    try:
        prods = s.ViewProducts(ViewProductsRequest(), metadata=meta)
        
        ids = {}
        for p in prods.products:
            ids[p.name.lower()] = p.product_id
        
        cart = s.CreateShoppingCart(CreateShoppingCartRequest(), metadata=meta)
        cid = cart.cart_id
        
        stuff = {
            'potato': 5,
            'sour cream': 1, 
            'dill': 1,
            'egg': 4
        }
        
        for item, num in stuff.items():
            pid = ids[item]
            req = AddProductToCartRequest(
                cart_id=cid,
                product_id=pid,
                quantity=num
            )
            s.AddProductToCart(req, metadata=meta)
        
        resp = s.Checkout(CheckoutRequest(cart_id=cid), metadata=meta)
        
        print("done!")
        print("next:", resp.instructions)
        
    except grpc.RpcError as e:
        print(f"failed: {e}")
    finally:
        c.close()

if __name__ == "__main__":
    main()
