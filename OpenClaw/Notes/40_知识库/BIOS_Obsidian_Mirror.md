# BIOS Obsidian Mirror

- Source vault: `E:\Note\BIOSNote\BIOS`
- Mirror folder: `workspace notes -> knowledge-base section -> BIOS_Vault`
- Purpose: expose BIOS and UEFI study notes to OpenClaw inside its own workspace
- Sync command: `powershell -ExecutionPolicy Bypass -File C:\Users\yqr\.openclaw\workspace\.openclaw\scripts\sync-bios-obsidian.ps1`

Current sync rules:

- only sync Markdown notes
- exclude `.git` and `.obsidian`
- exclude `OpenClaw`, `English_Learn`, `Python Notes`
- exclude heavy reference and attachment folders: `80-Reference`, `90-Assets`
- exclude `99-Archive`
- exclude obvious off-topic notes if they appear in BIOS folders

When answering BIOS, UEFI, EDK2, Setup menu, Skia UI, or firmware-debug questions, prefer searching this mirror first.
