//Created on Jul 3, 2005

package sitename;
import java.util.LinkedList;
import java.util.ListIterator;
import java.io.StringReader;
// @author Sergey Mirkin
public class getisbn {
    public static void main(String[] args) {
        LinkedList list;
	    String isbn = args[0];
	    try{
		    list = new LinkedList();
			//creating a request
			String uri = new AmazonXML().construct_ItemLookupURL(isbn,AmazonXML.ALL);
			//sending a request
			String response = RestRequest.issueRequest(uri);
			StringReader reader = new StringReader(response);
			//interpreting xml
			XMLConverter xml = new XMLConverter(reader);
			list.add(new Object[]{"isbn",isbn});
			list.add(new Object[]{"title",xml.getValue(AmazonXML.ITEMLOOKUP_TITLE)});
			list.add(new Object[]{"author",xml.getValue(AmazonXML.ITEMLOOKUP_AUTHOR)});
			list.add(new Object[]{"listprice",xml.getValue(AmazonXML.ITEMLOOKUP_LISTPRICE)});
			list.add(new Object[]{"publisher",xml.getValue(AmazonXML.ITEMLOOKUP_PUBLISHER)});
			list.add(new Object[]{"binding",xml.getValue(AmazonXML.ITEMLOOKUP_BINDING)});
			list.add(new Object[]{"pages",xml.getValue(AmazonXML.ITEMLOOKUP_PAGES)});
			list.add(new Object[]{"mimage",xml.getValue(AmazonXML.ITEMLOOKUP_MIMAGE_URL)});
			list.add(new Object[]{"simage",xml.getValue(AmazonXML.ITEMLOOKUP_SIMAGE_URL)});
			list.add(new Object[]{"limage",xml.getValue(AmazonXML.ITEMLOOKUP_LIMAGE_URL)});
			ListIterator iter = list.listIterator();
			String output = "beg";
			boolean flag = false;
			while (iter.hasNext()){
			    if (flag){
			        output += ">:<";
			    }
			    flag = true;
			    Object[] obj = (Object[])iter.next();
			    if (obj[1] != null){
			        output += (String)obj[0]+"=>"+((String)obj[1]).replaceAll(">:<",">/:<");
			    }
			}
			output += "end";
			System.out.print(output);
		}catch (Exception e){
			log.writeException("isbn: "+isbn+"\n"+e.getMessage());
		}
	}	
}
