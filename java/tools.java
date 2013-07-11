//Created on Jul 3, 2005

package sitename;

// @author Sergey Mirkin
public class tools {
    public static String toSQL(String str){
        if (str == null || str == ""){
            return "NULL";
        }else{
            str = str.replaceAll("\\\\","\\\\\\\\");
            str = str.replaceAll("[']","\\\\'");
            str = str.replaceAll("[\"]","\\\\\"");
            return "'"+str+"'";
        }
    }
}
