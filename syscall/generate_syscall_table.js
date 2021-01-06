let fs = require('fs');

function	parse_type(nargs, args)
{
	const regex = /^(const\s*)?char\s*/;
	for (let i = 0; i < nargs; i++)
	{
		const str = new String(args[i]);

		if (i > 0)
			process.stdout.write(", ");
		if (str.match(regex))
			process.stdout.write(`ARG_STR`);
		else if (str.indexOf('*') > -1)
			process.stdout.write(`ARG_PTR`);
		else
			process.stdout.write(`ARG_INT`);
	}
}

fs.readFile('syscalls', 'utf8', (err, data) => {
	const lines = data.split("\n");

	process.stdout.write("#ifndef SYSCALL_TABLE_H\n#define SYSCALL_TABLE_H\n\n");
	process.stdout.write("# include \"includes/syscalls.h\"\n");
	process.stdout.write("# define MAX_SYSCALL_NUM " + lines.length + "\n\n");
	process.stdout.write("const struct syscall_entry	syscalls[] = {\n");
	lines.forEach((line, i) => {
		const lexemes = line.split("\t");
		const name = lexemes[0];
		const nargs = lexemes[1];
		const args = lexemes.slice(3);

		process.stdout.write(`\t{.name = "${name}", .nargs = ${nargs}, .args = { `);
		parse_type(nargs, args);
		process.stdout.write(" } }");
		if (i < lines.length - 1)
			process.stdout.write(',');
		process.stdout.write("\n");
	})
	process.stdout.write("};\n");
	process.stdout.write("\n#endif\n");
})