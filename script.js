function doGet(e){
  Logger.log("--- doGet ---");
  Logger.log("tag : "+e.parameters.tag); 
 var tag = "",
     value = "",
     long="",
     lat="",
     name="";
  
  try {
 
    // this helps during debuggin
    if (e == null){e={}; e.parameters = {tag:"5",value:"1"};}
 
    tag = e.parameters.tag;
    value = e.parameters.value;
 
    // save the data to spreadsheet
    save_data(tag, value,lat,long,name);
 
 
    return ContentService.createTextOutput("Wrote:\n  tag: " + tag + "\n  value: " + value);
 
  } catch(error) { 
    Logger.log(error);    
    return ContentService.createTextOutput("oops...." + error.message 
                                            + "\n" + new Date() 
                                            + "\ntag: " + tag +
                                            + "\nvalue: " + value);
  }  
}
 
// ------------------------------------------- Method to save given data to a sheet
function save_data(tag, value,lat,long,name){
  Logger.log("--- save_data ---"); 
 
 
  try {
    var dateTime = new Date();
 
    // Paste the URL of the Google Sheets starting from https thru /edit
    // For e.g.: https://docs.google.com/..../edit 
    var ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1vKFmw0jANKxW9pZVAEKaQhpd3qral1SrKYyxbnJbfGU/edit");
    var sht = ss.getSheetByName("Log");
 
 
    
    var found=0;
    var row=1;
    var lastRow = sht.getLastRow() + 1;
    while(!found&&row<lastRow)                        //loop until found or last row
    {
      found=sht.getRange("A"+row).getValue()==tag;
      row++;
    }
    if(found){
      row--;
     // sht.getRange("E" + row).setValue(""); // value
    }
    else
    {
      row=lastRow;
     // sht.getRange("E" + lastRow).setValue("new"); 
    }
    
     
    // Start Populating the data
    sht.getRange("A" + row).setValue(tag); // ID
    sht.getRange("B" + row).setValue(dateTime); // dateTime
    sht.getRange("C" + row).setValue(lat);
    sht.getRange("D" + row).setValue(long);
    sht.getRange("E" + row).setValue(name);
    var New=sht.getRange("F" + row).getValue();
    New+=+value;
    sht.getRange("F" + row).setValue(New);
    var total=sht.getRange("G" + row).getValue();
    total+=+value;
    sht.getRange("G" + row).setValue(total);
    

 
  }
 
  catch(error) {
    Logger.log(JSON.stringify(error));
  }
 
  Logger.log("--- save_data end---"); 
}
