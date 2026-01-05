-- Project-local raylib build & run

vim.keymap.set("n", "<leader>b", function()
  vim.cmd("!./build.sh")
end, { desc = "Build raylib project" })

vim.keymap.set("n", "<leader>r", function()
  vim.cmd("!./run.sh")
end, { desc = "Run raylib app" })
