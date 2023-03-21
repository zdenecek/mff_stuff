// $Id: TYPE.java 306757 2005-10-06 11:33:33 +0530 (Thu, 06 Oct 2005) rana_b $
/*
 * Copyright 2004 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.ftpserver.command;

import java.io.IOException;

import org.apache.ftpserver.Command;
import org.apache.ftpserver.FtpRequestImpl;
import org.apache.ftpserver.FtpWriter;
import org.apache.ftpserver.RequestHandler;

/**
 * <code>TYPE &lt;SP&gt; &lt;type-code&gt; &lt;CRLF&gt;</code><br>
 *
 * The argument specifies the representation type.
 * 
 * @author <a href="mailto:rana_b@yahoo.com">Rana Bhattacharyya</a>
 */
public 
class TYPE implements Command {
    
    /**
     * Execute command
     */
    public void execute(RequestHandler handler,
                        FtpRequestImpl request, 
                        FtpWriter out) throws IOException {
        
        // reset state variables
        request.resetState();
        
        // get type from argument
        char type = 'A';
        if (request.hasArgument()){
            type = request.getArgument().charAt(0);
            type = Character.toUpperCase(type);
        }
        
        // set type
        if( (type == 'A') || (type == 'I') ) {
            handler.setDataType(type);
            out.send(200, "TYPE", null);
        }
        else {
            out.send(504, "TYPE", null);
        }
    }
    
}
