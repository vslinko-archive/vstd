{
    "targets": [
        {
            "target_name": "vstd",
            "type": "executable",
            "sources": [
                "src/vstd/adt/__tests__/list_test.c",
                "src/vstd/adt/list.c",
                "src/vstd/test.c",
                "src/vstd/test_runner.c"
            ],
            "include_dirs": [
              "src"
            ]
        }
    ]
}
