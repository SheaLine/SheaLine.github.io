function openModal(modalId, projectId) {
    document.getElementById(modalId).style.display = 'block';
    populateFiles(modalId, 'project-files/', projectId);
}

function closeModal(modalId) {
    document.getElementById(modalId).style.display = 'none';
}

function populateFiles(modalId, projectPath, projectId) {
    // Assume you have an array of files for the project
    var files;
    if (projectId == 'ToasterOven'){
        files = ['README.pdf', 'Leds.h', 'toasterOven_main.c'];
    }
    else if (projectId == 'LinkedList&GraphADT'){
        files = ['List.h', 'List.c', 'Graph.h', 'Graph.c', 'GraphTest.c', 'FindPath.c', 'Makefile.txt', 'README.txt']
    }
    else if (projectId == 'Dictionary'){
        files = ['Dictionary.h', 'Dictionary.cpp', 'DictionaryTest.cpp', 'Order.cpp', 'WordFrequency.cpp', 'Makefile.txt', 'README.txt']
    }
    else if (projectId == 'SDN'){
        files = ['controller.py', 'topo.py', 'topo-diagram.PNG', 'README.txt']
    }

    var fileList = document.getElementById('fileList' + projectId);
    fileList.innerHTML = '';

    files.forEach(function(file) {
        var li = document.createElement('li');
        var a = document.createElement('a');
        a.textContent = file;
        a.href = projectPath + projectId + '/' + file;
        a.target = '_blank'; // open in new tab
        li.appendChild(a);
        fileList.appendChild(li);
    });
}
