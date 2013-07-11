//Created on May 2, 2005

package sitename;
import java.sql.*;
// @author Sergey Mirkin
public class SendMailQueue{
    public static void main(String[] args) throws Exception{
        Connection con = null;
		try{
			// Load database driver if not already loaded
			Class.forName(StringInterface.DRIVER);
			// Establish network connection to database
			con = DriverManager.getConnection(StringInterface.DBURL,
			        StringInterface.DBUSERNAME,"fignamoya");
			Statement st = con.createStatement();
			ResultSet rs = st.executeQuery("SELECT * from mail ORDER BY `date` LIMIT 6");
			PreparedStatement pst = con.prepareStatement(
			        "DELETE from mail ORDER BY `date` LIMIT 6");
			boolean firstTime = true;
			SendMail mail = null;
			while(rs.next()){
			    if (firstTime){
			        mail = new SendMail();
			    }
			    firstTime = false;
			    String id = rs.getString(0);
			    String fullMessage = rs.getString(1);
			    boolean ishtml = false;
			    if (fullMessage.replaceAll("^(.+[^\\])<<delim>>","$1").equals("true")){
			       ishtml = true;
			    }
			    String toEmail = fullMessage.replaceAll("^.+[^\\]<<delim>>(.+[^\\])<<delim>>.+$","$1");
			    String subject = fullMessage.replaceAll("^.+[^\\]<<delim>>.+[^\\]<<delim>>(.+[^\\])<<delim>>.+$","$1");
			    String message = fullMessage.replaceAll("^.+[^\\]<<delim>>.+[^\\]<<delim>>.+[^\\]<<delim>>(.+)$","$1");
			    message = message.replaceAll("<<n>>","\n");
			    String[] to = toEmail.split("<<and>>");
			    mail.send(to,message,subject,ishtml);
			}
			pst.executeUpdate();
		}catch(Exception e){
		    log.writeException(e.getMessage());
		    throw e;
		}finally{
			con.close();
			con = null;
		}
    }
}
