
function change_table_row_color(a){
    document.getElementById("table_row_"+a).classList.remove("analysis_results_row_normal", "analysis_results_row_anomaly");

    var row_class = "";
    if (document.getElementById("table_check_button_"+a).checked) {
        row_class = "analysis_results_row_anomaly";
    }
    else {
        row_class = "analysis_results_row_normal";
    }

    document.getElementById("table_row_"+a).classList.add(row_class);
}
