function get_function_pointer(string lpc){
  function tmp = function(string lpc){
    function tmp;
    write_file("/secure/functemp.c", sprintf("mixed bing(){return %s;}", lpc));
    catch{
      tmp = "/secure/functemp"->bing();
      destruct(find_object("/secure/functemp"));
    };

    rm("/secure/functemp.c");
    return tmp;
  };
  return bind(this_object()->unguarded((:evaluate($(tmp), $(lpc)):)), previous_object());
}
                   
    
